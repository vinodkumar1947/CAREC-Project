# ADR-001: Adopt Seeed tf_module_ops Architecture for CAREC Firmware Modules

## Status
**Accepted** — May 14, 2026

## Context

CAREC's initial firmware (`firmware/main/`) uses direct function calls between header-only modules (`sensecraft_detection.h`, `distance_estimator.h`, `directional_beep_patterns.h`, etc.) wired together in a 100 ms loop in `CAREC_main.cpp`. This is simple and works for bring-up, but diverges from Seeed's prescribed extensibility pattern.

Seeed's [Software Framework](https://wiki.seeedstudio.com/watcher_software_framework/) defines a three-layer architecture where all device features are implemented as **Task Flow modules** — pluggable C structs that conform to the `tf_module_ops` vtable. Modules communicate only via `tf_event_post()` / `tf_event_handler_register()` over a dedicated `esp_event` loop managed by `tf_engine`. The engine wires modules together from a JSON task-flow graph delivered by SenseCraft cloud or hardcoded locally.

**Why this matters for CAREC:**
- Without conformance, CAREC cannot receive task-flow graphs from SenseCraft app/cloud.
- Without conformance, we cannot reuse Seeed's existing `tf_module_ai_camera` (already handles SSCMA inference over SPI — the most complex piece).
- Non-conformant firmware drifts further from upstream BSP with each release, making future updates harder.

## Decision

Migrate all CAREC functional modules to the `tf_module_ops` vtable pattern, using the Seeed `task_flow_engine` as the execution host.

## The Seeed ABI (exact, from BSP source)

### tf_module.h

```c
struct tf_module_ops {
    int (*start)(void *p_module);
    int (*stop)(void *p_module);
    int (*cfg)(void *p_module, cJSON *p_json);
    int (*msgs_sub_set)(void *p_module, int evt_id);
    int (*msgs_pub_set)(void *p_module, int output_index, int *p_evt_id, int num);
};

typedef struct {
    const struct tf_module_ops *ops;
    void *p_module;
} tf_module_t;

typedef struct tf_module_mgmt {
    tf_module_t *(*tf_module_instance)(void);
    void (*tf_module_destroy)(tf_module_t *p_module);
} tf_module_mgmt_t;
```

### tf.h (key functions)

```c
// Registration
esp_err_t tf_module_register(const char *p_name, const char *p_desc,
                             const char *p_version, tf_module_mgmt_t *mgmt_handle);

// Inter-module events (producer posts, consumer handles)
esp_err_t tf_event_post(int32_t event_id, const void *event_data,
                        size_t event_data_size, TickType_t ticks_to_wait);
esp_err_t tf_event_handler_register(int32_t event_id,
                                    esp_event_handler_t event_handler, void *arg);
esp_err_t tf_event_handler_unregister(int32_t event_id, esp_event_handler_t handler);

// Engine lifecycle
esp_err_t tf_engine_init(void);
esp_err_t tf_engine_run(void);
esp_err_t tf_engine_stop(void);
esp_err_t tf_engine_flow_set(const char *p_str, size_t len);  // JSON task-flow graph
```

### tf_module_data_type.h (key payload types)

```c
// Type tag sent with every event
enum {
    TF_DATA_TYPE_DUALIMAGE_WITH_INFERENCE = 13,
    TF_DATA_TYPE_DUALIMAGE_WITH_INFERENCE_AUDIO_TEXT = 14,
};

typedef struct tf_data_dualimage_with_inference {
    uint32_t type;                        // TF_DATA_TYPE_DUALIMAGE_WITH_INFERENCE
    struct tf_data_image img_small;       // thumbnail (base64)
    struct tf_data_image img_large;       // full frame (base64)
    struct tf_data_inference_info inference;  // bboxes / classes from SSCMA
} tf_data_dualimage_with_inference_t;
```

### Memory rule

Producer allocates the payload. Consumer **must** call `tf_data_free(event_data)` (from `tf_module_util.h`) before returning from the event handler.

## CAREC Module Mapping

| Current header | tf_module role | Strategy |
|---|---|---|
| `sensecraft_detection.h` | Excitation Source | **Replace** with Seeed's `tf_module_ai_camera` directly — it already handles SSCMA over SPI2, model OTA, dual-image output. No custom code needed. |
| `motion_detector.h` | Excitation Source / gate | Inline into `tf_module_ai_camera` params (it has a `trigger` config field) or keep as a pre-filter in a CAREC Trigger module. |
| `distance_estimator.h` + zone logic | Trigger Module | New `tf_module_carec_detector`: subscribes to `TF_DATA_TYPE_DUALIMAGE_WITH_INFERENCE`, computes zone (RED/YELLOW/GREEN), publishes a `tf_data_carec_zone_t` event. |
| `display_alert.h` + `directional_beep_patterns.h` | Alarm Module | New `tf_module_carec_alert`: subscribes to zone events, drives LED + speaker. |
| `ble_logger.h` | Alarm Module | New `tf_module_carec_ble`: subscribes to zone events, posts JSON to BLE GATT. |
| `wifi_ota.h` | APP layer | Keep outside task-flow — OTA is an app-layer concern, not a module. |

## Task-Flow JSON (hardcoded for CAREC)

```json
{
  "tlid": 1,
  "ctd": 1,
  "tn": "CAREC Obstacle Detection",
  "type": 0,
  "task_flow": [
    {"id": 1, "type": "ai camera",        "version": "1.0.0", "index": 0,
     "params": {"model_id": 0, "modes": 1},
     "wires": [[2, 3]]},
    {"id": 2, "type": "carec detector",   "version": "1.0.0", "index": 0,
     "params": {"dist_red": 60, "dist_yellow": 100},
     "wires": [[3, 4]]},
    {"id": 3, "type": "carec alert",      "version": "1.0.0", "index": 0,
     "params": {"sound": true, "rgb": true},
     "wires": [[]]},
    {"id": 4, "type": "carec ble",        "version": "1.0.0", "index": 0,
     "params": {},
     "wires": [[]]}
  ]
}
```

## Migration Plan

### Prerequisites (before any module code)
1. Add `examples/factory_firmware/main/task_flow_engine/` from Seeed BSP as an IDF component in `firmware/components/task_flow_engine/`
2. Add `examples/factory_firmware/main/task_flow_module/common/` (data types + util)
3. Verify it compiles against IDF v5.3.5 (upstream uses v5.2.1 — `esp_event` API is backward-compatible)
4. Replace `CAREC_main.cpp` app_main with `tf_engine_init()` + `tf_engine_run()` + module registration

### Phase 1 — Validator (one new module, keep old loop)
- Add `tf_module_carec_detector` in `firmware/main/task_flow_module/`
- Wire it to `tf_module_ai_camera` output in a test task-flow JSON
- Verify zone events publish correctly; old loop still runs in parallel as fallback

### Phase 2 — Alert modules
- Add `tf_module_carec_alert` (LED + speaker) and `tf_module_carec_ble`
- Remove old direct-call loop from `CAREC_main.cpp`

### Phase 3 — Cleanup
- Delete old header stubs (`sensecraft_detection.h`, `motion_detector.h` loop code)
- Run 50-obstacle test matrix against the new architecture

## Alternatives Considered

- **Stay with direct-call loop** — simpler, brings up faster, but permanently cuts off SenseCraft task-flow integration and forces hand-maintenance of every API the BSP exposes.
- **Partial conformance (only alarm modules)** — reduces value; the key win is reusing `tf_module_ai_camera` to eliminate our SSCMA hand-roll.

## Consequences

- **Positive:** Reuse `tf_module_ai_camera` (most complex module, already has model OTA, dual-image, condition triggers). Full SenseCraft cloud task-flow compatibility. Clean module boundaries and memory ownership.
- **Negative:** Requires pulling in Seeed's `task_flow_engine` source (~1,500 LOC of C). ESP-IDF v5.2.1 vs v5.3.5 compatibility must be verified. Migration spans ~1 week of focused work.
- **Timing risk:** Hardware bring-up (beep + LED wiring, LCD debug) should complete first. Start migration after `carec_loop` is validated end-to-end.

## References

- Software Framework: https://wiki.seeedstudio.com/watcher_software_framework/
- Module Dev Guide: https://wiki.seeedstudio.com/watcher_function_module_development_guide/
- Seeed BSP source: https://github.com/Seeed-Studio/SenseCAP-Watcher-Firmware (branch: main, path: `examples/factory_firmware/main/`)
