# CAREC Firmware API Reference

**Firmware version:** 0.2.0  
**Platform:** ESP32-S3 / ESP-IDF v5.3.5  
**Entry point:** `firmware/main/CAREC_main.cpp`  
**Updated:** May 15, 2026

All modules are header-only `.h` files in `firmware/main/`. Include order and initialization sequence is defined in `CAREC_main.cpp → carec_setup()`.

---

## `sensecraft_detection.h` — SSCMA / Himax HX6538 Inference

```cpp
#include "sensecraft_detection.h"
```

### Types

```cpp
struct Detection {
    char  label[32];     // COCO class name ("person", "chair", "couch", …)
    float confidence;    // 0.0–1.0
    float bbox_x;        // normalised centre-x (0.0–1.0)
    float bbox_y;        // normalised centre-y (0.0–1.0)
    float bbox_w;        // normalised width    (0.0–1.0) — used for distance heuristic
    float bbox_h;        // normalised height   (0.0–1.0)
};

#define MAX_DETECTIONS 8

struct DetectionResult {
    Detection items[MAX_DETECTIONS];   // up to 8 detections, ordered by descending confidence
    int       count;                   // number of valid entries in items[]
    uint32_t  inference_ms;            // wall time for last SSCMA invoke
};

#define DETECTION_CONFIDENCE_THRESHOLD 0.55f
```

### Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `sensecraft_init` | `bool sensecraft_init()` | PCA9535 power-on sequence → Himax WE2 reset → SSCMA SPI2 client init. Call once in `carec_setup()`. Returns `false` on I²C/SPI failure. |
| `sensecraft_detect` | `DetectionResult sensecraft_detect()` | Invoke SSCMA inference on the WE2. Blocks ~76 ms (measured). Returns populated `DetectionResult`. |
| `log_detections` | `void log_detections(DetectionResult*)` | Print all detections to Serial (label + confidence + bbox). |
| `log_distances` | `void log_distances(DetectionResult*)` | Print estimated distances to Serial (uses `distance_estimator.h` heuristic). |

### Hardware

| Signal | GPIO | Notes |
|--------|------|-------|
| SPI2 SCK | 4 | BSP_SPI2_HOST_SCLK |
| SPI2 MISO | 6 | BSP_SPI2_HOST_MISO |
| SPI2 MOSI | 5 | BSP_SPI2_HOST_MOSI |
| SPI2 CS | 21 | BSP_SSCMA_CLIENT_SPI_CS |
| PCA9535 SDA | 47 | I²C0 (shared with ES8311) |
| PCA9535 SCL | 48 | I²C0 |

---

## `distance_estimator.h` — Distance Heuristic

```cpp
#include "distance_estimator.h"
```

### Constants (see also `firmware/config/zone_config.h`)

| Constant | Default | Meaning |
|----------|---------|---------|
| `DIST_MIN_CM` | 10.0 | Clamp minimum output |
| `DIST_MAX_CM` | 200.0 | Clamp maximum (returned when no detections) |
| `CALIB_BBOX_AT_RED` | 0.241 | Expected `bbox_w` (normalised) at 60 cm boundary |
| `CALIB_BBOX_AT_YELLOW` | 0.144 | Expected `bbox_w` (normalised) at 100 cm boundary |

### Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `nearest_obstacle_cm` | `float nearest_obstacle_cm(DetectionResult*)` | Returns distance in cm to the closest detection using bbox_w heuristic. Returns `DIST_MAX_CM` when `count == 0`. |

---

## `motion_detector.h` — Motion State

```cpp
#include "motion_detector.h"
```

> **Phase 1 note:** Optical flow is not available on the Watcher — the OV5647 connects to the Himax HX6538 over MIPI; the ESP32-S3 never receives raw frames. All functions return `MOTION_FORWARD` (always-active detection). Phase 2 will replace this with a Grove ADXL345 accelerometer gate when stationary suppression is needed.

### Types

```cpp
typedef enum {
    MOTION_STATIONARY = 0,
    MOTION_FORWARD    = 1,
    MOTION_BACKWARD   = 2,
} MotionState;
```

### Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `motion_init` | `void motion_init()` | Prints Phase 1 notice. No hardware init needed. |
| `motion_update` | `MotionState motion_update()` | Always returns `MOTION_FORWARD` in Phase 1. |
| `motion_is_active` | `bool motion_is_active()` | Always returns `true` in Phase 1. |

---

## `directional_beep_patterns.h` — Speaker Alerts

```cpp
#include "directional_beep_patterns.h"
```

### Constants

| Constant | Value | Pattern |
|----------|-------|---------|
| `BEEP_NONE` | -1 | Silence |
| `BEEP_WARNING` | 0 | 2 beeps/sec at 1500 Hz (YELLOW zone) |
| `BEEP_CRITICAL` | 1 | Rapid burst at 2000 Hz (RED zone) |

### Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `beep_init` | `bool beep_init()` | I²S0 + ES8311 codec init (full DAC power-up + speaker route). Returns `false` on I²C failure. Call once in `carec_setup()`. |
| `beep_trigger` | `void beep_trigger(int pattern)` | Set active beep pattern. Changing mid-sequence restarts immediately. |
| `beep_update` | `void beep_update()` | Advance state machine — writes PCM chunks to I²S DMA. **Call every loop iteration.** |
| `beep_ready` | `bool beep_ready()` | Returns `true` if ES8311 init succeeded. |
| `beep_play_tone` | `void beep_play_tone(uint32_t freq_hz)` | Low-level: play a tone at frequency. Used internally and by test modes. |
| `beep_play_silence` | `void beep_play_silence()` | Low-level: write silence samples to I²S. |

### Hardware

| Signal | GPIO | Notes |
|--------|------|-------|
| I²S MCLK | 10 | 4.096 MHz (256 × 16 kHz) |
| I²S BCLK | 11 | |
| I²S LRCK | 12 | |
| I²S DOUT | 16 | DAC to ES8311 |
| ES8311 SDA | 47 | I²C0 (shared with PCA9535) |
| ES8311 SCL | 48 | I²C0 |
| ES8311 addr | 0x18 | 7-bit |

---

## `display_alert.h` — SPD2010 QSPI LCD Driver

```cpp
#include "display_alert.h"
```

> **Status:** Driver initialises but panel stays dark — suspected BL driver IC / ribbon cable issue. Use `led_status.h` for visual zone feedback in the meantime. Kept in code as `display_set_zone`/`display_update` are no-ops when dark.

### Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `display_init` | `void display_init()` | PCA9535 power-on + SPD2010 QSPI driver init + PWM backlight. |
| `display_set_zone` | `void display_set_zone(int zone)` | Set target zone. No hardware update until `display_update()`. |
| `display_update` | `void display_update()` | Advance blink state machine. GREEN = solid. YELLOW = 500 ms blink. RED = 200 ms blink. **Call every loop iteration.** |

### Zone constants

```cpp
#define ZONE_GREEN  0   // >100 cm — clear
#define ZONE_YELLOW 1   // 60–100 cm — caution
#define ZONE_RED    2   // <60 cm — danger
```

---

## `led_status.h` — WS2813 RGB Zone Indicator

```cpp
#include "led_status.h"
```

Primary visual feedback while LCD is offline. Mirrors `display_alert.h` zone semantics on the onboard WS2813 mini LED (GPIO 40).

| Zone | LED colour | Blink |
|------|-----------|-------|
| `ZONE_GREEN` | Green solid | none |
| `ZONE_YELLOW` | Yellow | `DISPLAY_BLINK_YELLOW_MS` period |
| `ZONE_RED` | Red | `DISPLAY_BLINK_RED_MS` period |

### Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `led_status_init` | `bool led_status_init()` | Init WS2813 RMT driver on GPIO 40. Returns `false` on failure. |
| `led_status_set_zone` | `void led_status_set_zone(int zone)` | Switch to zone. Solid colours apply immediately; blink zones start ON. |
| `led_status_update` | `void led_status_update()` | Advance blink state machine. **Call every loop iteration.** |

---

## `ble_logger.h` — BLE GATT Event Logger

```cpp
#include "ble_logger.h"
```

Implements a Nordic UART Service (NUS) GATT server. The caregiver's phone subscribes to the TX characteristic and receives real-time JSON obstacle alerts. When no client is connected, events mirror to Serial.

### Event format

```json
{"zone":"RED","dist_cm":42.3,"motion":"FORWARD","ts_ms":12345678}
```

### UUIDs (Nordic UART Service)

| UUID | Purpose |
|------|---------|
| `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` | NUS Service |
| `6E400003-B5A3-F393-E0A9-E50E24DCCA9E` | TX — phone reads events here (notify) |
| `6E400002-B5A3-F393-E0A9-E50E24DCCA9E` | RX — reserved for Phase 2 commands |

### Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `ble_logger_init` | `void ble_logger_init()` | Init BLE stack + GATT server. Device advertises as `CAREC-Watcher`. |
| `ble_log_event` | `void ble_log_event(float dist_cm, int zone, MotionState motion)` | Build + send JSON event via TX characteristic (or Serial if no client). |
| `ble_log_raw` | `void ble_log_raw(const char* msg)` | Send arbitrary string (debug / custom). |

---

## `wifi_ota.h` — WiFi + HTTPS OTA

```cpp
#include "wifi_ota.h"
```

### Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `ota_set_safe` | `void ota_set_safe(bool safe)` | Gate OTA checks. Must be `true` before `ota_check_and_update()` does anything. |
| `ota_check_and_update` | `void ota_check_and_update()` | Poll OTA server. If new version available and safe flag is set, downloads + flashes + reboots. No-op when WiFi disconnected or flag is false. |

### Safety gate

```cpp
// OTA only in GREEN zone (no obstacle threat)
ota_set_safe(zone == ZONE_GREEN);
ota_check_and_update();
```

---

## Bring-up test modes (`rgb_led_test.h`, `speaker_test.h`)

These are mutually exclusive bring-up modes activated by `#define` in `CAREC_main.cpp`. Leave all commented for full firmware.

| Flag | Header | What it does |
|------|--------|-------------|
| `LED_TEST` | `rgb_led_test.h` | WS2813 25-colour cycle on GPIO 40 (2 s on / 58 s off). Validated. |
| `SPEAKER_TEST` | `speaker_test.h` | 1500 Hz heartbeat beep once per minute. Validated. |
| `DISPLAY_TEST` | `display_alert.h` | SPD2010 RED → YELLOW → GREEN cycle. Panel stays dark — deferred. |

---

## `carec_setup()` / `carec_loop()` call order

```
carec_setup() — called once from app_main():
  led_status_init()    GPIO 40 WS2813 zone indicator
  sensecraft_init()    PCA9535 power-on → Himax WE2 reset → SSCMA SPI2 client
  display_init()       SPD2010 QSPI LCD (currently dark — kept for future fix)
  beep_init()          ES8311 codec + I²S0
  motion_init()        Phase 1 always-forward notice
  ble_logger_init()    BLE GATT Nordic UART Service
  wifi_connect()       WiFi (non-blocking; OTA disabled on fail)

carec_loop() — 10 Hz (100 ms delay):
  1. sensecraft_detect()            → DetectionResult
  2. log_detections() + log_distances()
  3. nearest_obstacle_cm()          → float dist_cm
  4. Classify zone (RED/YELLOW/GREEN)
  5. beep_trigger(zone_to_beep(zone)) + beep_update()
  6. display_set_zone(zone) + display_update()
  7. led_status_set_zone(zone) + led_status_update()
  8. ble_log_event(dist_cm, zone, MOTION_FORWARD)
  9. ota_set_safe(zone == ZONE_GREEN) + ota_check_and_update()
 10. vTaskDelay(100 ms)
```
