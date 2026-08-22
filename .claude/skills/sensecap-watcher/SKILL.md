---
name: sensecap-watcher
description: |
  CAREC project — SenseCAP Watcher W1-A embedded firmware specialist. Covers SSCMA client API
  (Himax HX6538 inference over SPI2), PCA9535 power sequencing, ES8311 codec register sequences,
  SPD2010 QSPI LCD, WS2813 RGB LED, tf_module_ops vtable migration (ADR-001), and ESP-IDF v5.3.5
  component/CMake conventions for this specific board. Activates when any of these are mentioned:
  SSCMA, sscma_client, Seeed_Arduino_SSCMA, tf_module, tf_event_post, task_flow, PCA9535, ES8311,
  SPD2010, Himax, HX6538, WiseEye2, WE2, SenseCraft detection, AI camera module.
context: inline
---

# SenseCAP Watcher W1-A — CAREC Embedded Specialist

You are working on the CAREC project. The target hardware is the **SenseCAP Watcher W1-A** (ESP32-S3 + Himax HX6538 NPU). All source lives in `firmware/main/`. Toolchain is **ESP-IDF v5.3.5** — no Arduino IDE, no arduino-cli.

## Hardware Quick Reference

| Peripheral | Pins | Notes |
|-----------|------|-------|
| PCA9535 I²C0 | SDA=47, SCL=48, addr=0x21 | IO expander: power rails + SSCMA RST |
| Himax SSCMA SPI2 | SCK=4, MISO=6, MOSI=5, CS=21 | 12 MHz, FSPI=SPI2_HOST |
| ES8311 codec I²C | SDA=47, SCL=48, addr=0x18 | Shared I²C bus with PCA9535 |
| I²S (speaker) | MCLK=10, BCLK=11, LRCK=12, DOUT=16 | 16 kHz / 16-bit mono |
| WS2813 RGB LED | GPIO 40 | RMT backend, single pixel, 3.3V |
| SPD2010 QSPI LCD | CLK=7, D0=9, D1=1, D2=14, D3=13, CS=45, BL=8 | **Currently dark — deferred** |
| Touch CHSC6x | SDA=39, SCL=38 | |
| OV5647 camera | MIPI to Himax | ESP32-S3 does NOT see raw frames |

## PCA9535 Power-On Sequence

Critical — mirrors `bsp_board_init()` in Seeed BSP. All rails off before start:

```c
// Port 1: configure all as outputs except bit 5 (BAT_DET input)
Wire.begin(47, 48, 400000UL);
_pca9535_write(PCA9535_REG_CFG1, 0x20);   // bit 5 = input, rest output
// All outputs = 0 (power off)
_pca9535_write(PCA9535_REG_OUT1, 0x00);
// Step 1: EXP_PWR_SYSTEM (bit 2 of port 1) = 1
_pca9535_write(PCA9535_REG_OUT1, EXP_PWR_SYSTEM);
vTaskDelay(pdMS_TO_TICKS(100));
// Step 2: all startup rails on (AI_CHIP + LCD + CODEC + GROVE + etc.)
_pca9535_write(PCA9535_REG_OUT1, EXP_PWR_STARTUP);
vTaskDelay(pdMS_TO_TICKS(200));
// Step 3: Himax WE2 reset (PIN_7 = BSP_SSCMA_CLIENT_RST, active-low)
_pca9535_write(PCA9535_REG_OUT0, 0x00);   // RST low
vTaskDelay(pdMS_TO_TICKS(50));
_pca9535_write(PCA9535_REG_OUT0, 0x80);   // RST high (bit 7)
vTaskDelay(pdMS_TO_TICKS(800));            // WE2 SSCMA firmware boot time
```

## ES8311 Codec Init

Must run **both** init pass and DAC-start pass (from esp-adf `es8311_codec_init` + `es8311_start(ES_MODULE_DAC)`). Partial init leaves codec silent:

```c
// Init pass (clock, sysclk, PLL, ADC)
_es8311_write(0x00, 0x1F);  // reset
_es8311_write(0x01, 0x30);  // sysclk div
_es8311_write(0x02, 0x10);  // MCLK source
// ... (see directional_beep_patterns.h for full sequence)

// DAC-start pass — REQUIRED for audio output
_es8311_write(0x0E, 0x02);  // DAC analog power-up
_es8311_write(0x12, 0x00);  // DAC digital power
_es8311_write(0x0D, 0x01);  // DAC output enable
_es8311_write(0x15, 0x40);  // speaker output route
_es8311_write(0x32, 0xBF);  // DAC volume = 0 dB (NOT 0x00 which is mute)
_es8311_write(0x31, 0x00);  // unmute (clear mute bits)
```

## SSCMA Client API (Seeed_Arduino_SSCMA)

```cpp
#include <Seeed_Arduino_SSCMA.h>

SSCMA ai;
SPIClass spi(FSPI);  // FSPI = SPI2_HOST, separate from display SPI3_HOST

// Init
spi.begin(SSCMA_SCK, SSCMA_MISO, SSCMA_MOSI, SSCMA_CS);
ai.begin(&spi, D7, -1, SSCMA_CS, 12000000);  // or similar — check vendored component

// Invoke inference
if (ai.invoke() == 0) {
    for (auto &box : ai.boxes()) {
        float bbox_w = box.w / SSCMA_SCALE;  // normalise to 0–1
        float conf   = box.score / 100.0f;
        uint8_t cls  = box.target;
    }
}
```

## tf_module_ops Migration (ADR-001)

**Status:** Planned for Week 3 — finish bring-up first.

See `docs/specifications/ADR-001-tf-module-ops-architecture.md` for the full plan.

### Vtable every new CAREC module must implement

```c
#include "tf_module.h"   // from task_flow_engine/include/

struct tf_module_ops {
    int (*start)(void *p_module);
    int (*stop)(void *p_module);
    int (*cfg)(void *p_module, cJSON *p_json);
    int (*msgs_sub_set)(void *p_module, int evt_id);
    int (*msgs_pub_set)(void *p_module, int output_index, int *p_evt_id, int num);
};

// Module struct: tf_module_t MUST be first member
typedef struct tf_module_carec_X {
    tf_module_t module_base;   // ← first member, required
    int input_evt_id;
    // ... your state
} tf_module_carec_X_t;
```

### Inter-module events

```c
// Publish (producer allocates, consumer calls tf_data_free)
tf_event_post(output_evt_id, &data, sizeof(data), pdMS_TO_TICKS(100));

// Subscribe (in msgs_sub_set)
tf_event_handler_register(evt_id, __event_handler, p_module);
```

### Register

```c
static tf_module_mgmt_t _mgmt = {
    .tf_module_instance = _instance,
    .tf_module_destroy  = _destroy,
};
tf_module_register("carec detector", "CAREC obstacle detector", "1.0.0", &_mgmt);
```

### Key data type

```c
// Primary input from tf_module_ai_camera
tf_data_dualimage_with_inference_t *p = (tf_data_dualimage_with_inference_t *)event_data;
// p->inference.p_data → sscma_client_box_t[] (cast based on p->inference.type)
// Always call tf_data_free(event_data) before returning from handler
```

## ESP-IDF v5.3.5 Notes

- System python3 on this Mac is 3.14 — prepend IDF venv first: `export PATH="$HOME/.espressif/python_env/idf5.3_py3.9_env/bin:$PATH"`
- Flash port: `/dev/cu.usbmodem5B142665543` (higher-numbered = ESP32-S3; lower = Himax console)
- Build: `cd firmware && idf.py build && idf.py -p /dev/cu.usbmodem5B142665543 flash monitor`
- New components: add to `firmware/main/idf_component.yml`, then `idf.py update-dependencies`

## Official References

- Software Framework (tf_module_ops): https://wiki.seeedstudio.com/watcher_software_framework/
- Module Dev Guide: https://wiki.seeedstudio.com/watcher_function_module_development_guide/
- Firmware BSP source: https://github.com/Seeed-Studio/SenseCAP-Watcher-Firmware
- OSHW schematics: https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher
