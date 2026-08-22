# Firmware — CAREC Wheelchair Safety System

**Hardware:** SenseCAP Watcher W1-A Clear Enclosure (ESP32-S3 + Himax WiseEye2 HX6538 NPU)
**Toolchain:** ESP-IDF v5.3.5 (with `arduino-esp32` as a managed component for `Arduino.h` / `Wire.h` / `BLEDevice` ergonomics)
**Language:** C++17

---

## Architecture

```
firmware/
├── CMakeLists.txt                  ← top-level IDF project
├── partitions.csv                  ← flash layout
├── sdkconfig.defaults              ← committed sdkconfig overrides
├── config/                         ← build_flags.h, zone_config.h, wifi_config.h (gitignored)
├── main/                           ← canonical app sources
│   ├── CMakeLists.txt
│   ├── idf_component.yml           ← ESP-IDF component dependencies
│   ├── CAREC_main.cpp              ← entry point (app_main)
│   ├── sensecraft_detection.h      ← SSCMA person detection over SPI2 (Himax HX6538)
│   ├── distance_estimator.h        ← bbox_w → distance heuristic
│   ├── display_alert.h             ← SPD2010 QSPI LCD zone colors (RED/YELLOW/GREEN)
│   ├── directional_beep_patterns.h ← ES8311 I²S audio alerts
│   ├── motion_detector.h           ← optical-flow motion gate (fwd/bwd/stationary)
│   ├── ble_logger.h                ← BLE GATT Nordic UART logging
│   └── wifi_ota.h                  ← WiFi + HTTPS OTA via esp_https_ota
├── components/Seeed_Arduino_SSCMA/ ← vendored Seeed SSCMA driver
├── managed_components/             ← IDF component manager output (gitignored)
├── tools/                          ← flash.sh, monitor.sh, backup_nvsfactory.sh
├── backups/                        ← factory NVS dumps (keep)
├── releases/                       ← release artifacts manifest
├── README.md                       ← this file
├── SETUP.md                        ← build/flash setup
├── RGB_LED_BRINGUP.md              ← standalone WS2813 LED test (W1-A bring-up)
└── SenseCAP_Watcher_W1A_Firmware_Programming_Guide.md
```

---

## Safety Logic (100ms loop)

```
1. Optical-flow motion gate (motion_detector.h):
   - Capture 80×60 grayscale thumbnail from OV5647
   - Track 12 corner features with Lucas-Kanade sparse optical flow
   - Classify: STATIONARY → skip to step 6 (no alerts, allow OTA)
               FORWARD / BACKWARD → continue to step 2

2. Camera captures frame (120° FOV, 5MP OV5647, fixed focus 3m)
3. SenseCraft NPU runs object detection (Himax HX6538)
4. Estimate distance to nearest obstacle (bounding-box heuristic)
5. Classify into alert zone:
   - 0–60cm   → ZONE_RED:    RED display fast blink (200ms) + rapid beeps
   - 60–100cm → ZONE_YELLOW: YELLOW display slow blink (500ms) + double beeps
   - 100+cm   → ZONE_GREEN:  GREEN display solid on + silence
6. Log event via BLE → SenseCraft Mate app
7. OTA check on WiFi reconnect (only in ZONE_GREEN)
```

### Motion Gate (Phase 1 stub — always-forward)

The OV5647 camera feeds the Himax HX6538 directly over MIPI CSI — the ESP32-S3 never receives raw frame data, so optical flow cannot be computed in firmware. The current `motion_detector.h` is a stub that always returns `MOTION_FORWARD`, keeping the detection loop always active.

| State | Scene flow | Action |
|-------|-----------|--------|
| STATIONARY | No/minimal flow | Display GREEN, silence, allow OTA |
| FORWARD | Diverging flow (scene expands) | Run detection + zone alerts |
| BACKWARD | Converging flow (scene contracts) | Run detection + zone alerts |

**Phase 2 plan:** Replace stub with an accelerometer (Grove IIC, ADXL345/MPU6050) for real motion gating — no camera dependency.

---

## Alert Zones

| Zone | Distance | Display (1.45" screen) | Speaker (1W) |
|------|----------|------------------------|--------------|
| GREEN | 100+cm | GREEN solid on | Silent |
| YELLOW | 60–100cm | YELLOW slow blink (500ms) | 2 beeps/sec, 1500Hz |
| RED | 0–60cm | RED fast blink (200ms) | Rapid burst, 2000Hz |

---

## Development Phases

| Phase | When | Goal |
|-------|------|------|
| Phase 1 | Hardware arrives (May 2026) | Validate SenseCraft object detection |
| Phase 2 | Week 2 | Implement distance thresholds + beep logic |
| Phase 3 | Week 3 | Custom firmware, LLM integration, OTA |
| Phase 4 | Week 4 | Field test on wheelchair, 50+ obstacle scenarios |

---

## Status

- [x] PCA9535 power sequencing → Himax WE2 boots
- [x] SSCMA person detection running (~76 ms inference, 58–83% conf)
- [x] BLE GATT JSON event stream (`ble_logger.h`)
- [x] Onboard WS2813 RGB LED (GPIO 40) — `LED_TEST` validated; see `RGB_LED_BRINGUP.md`
- [x] ES8311 + 1 W speaker — `SPEAKER_TEST` validated (1500 Hz heartbeat). Driver fix: full DAC power-up + speaker route + 0 dB volume; init now matches esp-adf reference.
- [x] Beep patterns in main loop — **build-validated** (`beep_trigger`/`beep_update` wired in `carec_loop`; state machine bug fixed May 18)
- [x] LED zone indicator in main loop — **build-validated** (`led_status_set_zone`/`led_status_update` wired in `carec_loop`)
- [ ] Distance threshold logic (`distance_estimator.h`) — verify on real obstacles after wheelchair mount
- [ ] Motion gate (`motion_detector.h`) — Phase 1 always-forward stub; Phase 2 will use accelerometer (OV5647→Himax over MIPI; ESP32-S3 has no raw frame access)
- [ ] WiFi OTA update pipeline (`wifi_ota.h`)
- [ ] **Display (SPD2010 LCD) — DEFERRED** — after 3 iterations (PWM backlight, pre-driven pin LOW, mirror, bus flag) the panel stays completely dark despite the driver reporting success. Suspect hardware (BL driver IC, ribbon seat, BL polarity). Falling back on WS2813 LED for visible feedback.
- [ ] 50-obstacle scenario test
- [ ] 8-hour battery runtime test

## Test modes (in `main/CAREC_main.cpp`)

Three build-time test modes, mutually exclusive (`#error` guard). Uncomment one; leave all commented for the full CAREC firmware.

| Flag | Source | Purpose |
|---|---|---|
| `DISPLAY_TEST` | `display_alert.h` | SPD2010 LCD RED → YELLOW → GREEN cycle (currently leaves panel dark) |
| `LED_TEST` | `rgb_led_test.h` | WS2813 25-color cycle on GPIO 40 (2 s on / 58 s off) |
| `SPEAKER_TEST` | `speaker_test.h` | 1500 Hz heartbeat beep once per minute on each LED-OFF transition |

## Hardware Reference

Official Seeed Studio repos used as BSP/schematic reference:

| Resource | URL | Purpose |
|----------|-----|---------|
| SenseCAP Watcher Firmware | https://github.com/Seeed-Studio/SenseCAP-Watcher-Firmware | BSP source: PCA9535 power sequencing, SPD2010 LCD driver, pin definitions |
| OSHW SenseCAP Watcher | https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher | Schematics (SenseCAP_Watcher_v1.0_SCH.pdf), datasheet links |

Key hardware facts confirmed from these sources:

| Peripheral | Detail |
|-----------|--------|
| LCD panel | SPD2010 (not ST7789) — 412×412 QSPI, SPI mode 3 |
| QSPI pins | CLK=GPIO7, D0=GPIO9, D1=GPIO1, D2=GPIO14, D3=GPIO13, CS=GPIO45 |
| Backlight | GPIO8, PWM via LEDC timer 1 at 5 kHz |
| RGB status LED | **GPIO40** — WS2813 mini (WS2812-protocol compatible), 3.3V, single addressable pixel. Drive via `led_strip` component (RMT backend). No PCA9535 power gating needed. See [`RGB_LED_BRINGUP.md`](RGB_LED_BRINGUP.md). |
| IO expander | PCA9535 at I2C addr 0x21, SDA=GPIO47, SCL=GPIO48 |
| Power seq. | BSP_PWR_SYSTEM on → 100ms → BSP_PWR_START_UP on → 50ms |
| Touch | CHSC6x, I2C SDA=GPIO39, SCL=GPIO38 |
| AI coprocessor | Himax HX6538 (WiseEye2), FSPI on reset-controlled rail |
| SPD2010 driver | `espressif/esp_lcd_spd2010` component, `#include "esp_lcd_spd2010.h"` |

## Official Seeed Documentation

Consult these before implementing any new feature or investigating a hardware API:

| Guide | URL | When to use |
|-------|-----|-------------|
| Product overview | https://wiki.seeedstudio.com/watcher/ | Device capabilities, integration ecosystem |
| Software Framework | https://wiki.seeedstudio.com/watcher_software_framework/ | `tf_module_ops` vtable, `tf_event_post`, task-flow JSON, `TF_DATA_TYPE_*` |
| Function Module Dev Guide | https://wiki.seeedstudio.com/watcher_function_module_development_guide/ | Adding a new `tf_module_*` to the task flow engine |
| UI Integration Guide | https://wiki.seeedstudio.com/watcher_ui_integration_guide/ | LVGL, `lv_pm_open_page()`, `lvgl_port_lock()`, view layer |
| SenseCraft App | https://wiki.seeedstudio.com/sensecap_app_introduction/ | BLE provisioning, HTTP notification block, cloud vs local AI |
| Service Framework | https://wiki.seeedstudio.com/watcher_software_service_framework/ | Data/Device Comm/Vision/Alert services, MQTT, deployment modes |
| Local AI Deployment | https://wiki.seeedstudio.com/watcher_local_deploy/ | Pointing Watcher at a local LLM/VLM server (not on-device) |
| Firmware Source (BSP) | https://github.com/Seeed-Studio/SenseCAP-Watcher-Firmware | Real API calls, SPD2010 driver, PCA9535 power sequence, factory firmware |
| OSHW Hardware | https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher | Schematics, BOM, datasheet links, factory flash recovery |

## See Also

- [Setup Guide](SETUP.md) — how to build and upload firmware
- [System Specification](../docs/specifications/system_spec.md) — full architecture
- [Hardware README](../hardware/README.md) — device specs and mounting
