# CAREC Programming Guide
## Build, Configure & Flash Firmware to SenseCAP Watcher W1-A

**Platform:** ESP32-S3 (SenseCAP Watcher W1-A Clear Enclosure)  
**Language:** C++17  
**Toolchain:** ESP-IDF v5.3.5 (pure IDF — no Arduino IDE, no arduino-cli, no PlatformIO)  
**Updated:** May 14, 2026

---

## Quick-Start (TL;DR)

```bash
# 1. Activate ESP-IDF (Python 3.9 venv required — see SETUP.md)
export PATH="$HOME/.espressif/python_env/idf5.3_py3.9_env/bin:$PATH"
. ~/esp/esp-idf/export.sh

# 2. Copy WiFi credentials (gitignored — never commit the real file)
cp firmware/config/wifi_config.h.template firmware/config/wifi_config.h
# Edit firmware/config/wifi_config.h with your SSID + password

# 3. Build
cd firmware/
idf.py build

# 4. Flash + monitor (higher-numbered USB port is ESP32-S3)
idf.py -p /dev/cu.usbmodem5B142665543 flash monitor
```

> See [`firmware/SETUP.md`](../../firmware/SETUP.md) for the Python 3.9 venv workaround required on this Mac (system python3 is 3.14, which breaks `export.sh` alone).

---

## Source File Map

```
firmware/
├── CMakeLists.txt                        ← top-level IDF project
├── partitions.csv                        ← flash partition table
├── sdkconfig.defaults                    ← committed sdkconfig overrides (target: esp32s3)
│
├── main/                                 ← ALL canonical app sources (IDF component)
│   ├── CMakeLists.txt
│   ├── idf_component.yml                 ← IDF component manager dependencies
│   ├── CAREC_main.cpp                    ← app_main() entry point — open this first
│   ├── sensecraft_detection.h            ← SSCMA person detection (Himax HX6538, SPI2)
│   ├── distance_estimator.h             ← bounding-box → distance heuristic
│   ├── display_alert.h                  ← SPD2010 QSPI LCD zone colors (RED/YELLOW/GREEN)
│   ├── directional_beep_patterns.h      ← ES8311 I²S audio alerts
│   ├── motion_detector.h                ← optical-flow motion gate (fwd/bwd/stationary)
│   ├── ble_logger.h                     ← BLE GATT Nordic UART JSON event stream
│   ├── wifi_ota.h                       ← WiFi + HTTPS OTA (esp_https_ota)
│   ├── rgb_led_test.h                   ← WS2813 25-color test cycle (LED_TEST mode)
│   ├── speaker_test.h                   ← ES8311 heartbeat beep (SPEAKER_TEST mode)
│   └── led_status.h                     ← Zone → LED color mapper (main loop)
│
├── components/
│   └── Seeed_Arduino_SSCMA/             ← vendored Seeed SSCMA driver (SPI protocol)
│
├── managed_components/                  ← IDF component manager output (gitignored)
│   ├── espressif__led_strip/            ← WS2813 RMT driver
│   ├── espressif__esp_lcd_spd2010/      ← SPD2010 QSPI LCD driver
│   └── ...
│
├── config/                              ← build-time configuration (edit these, not main/)
│   ├── wifi_config.h.template           ← safe template (committed)
│   ├── wifi_config.h                    ← YOUR credentials (gitignored — create from template)
│   ├── zone_config.h                    ← distance thresholds + calibration constants
│   └── build_flags.h                    ← feature on/off toggles + test mode selectors
│
└── tools/                               ← convenience scripts
    ├── flash.sh                         ← idf.py build + flash wrapper
    └── monitor.sh                       ← idf.py monitor with log capture
```

---

## Step 1 — Environment Setup

The toolchain is **ESP-IDF v5.3.5** only. No other workflow is supported.

### Install ESP-IDF (one-time)

```bash
git clone -b v5.3.5 --recursive https://github.com/espressif/esp-idf.git ~/esp/esp-idf
cd ~/esp/esp-idf && ./install.sh esp32s3
```

### Activate the environment (every new terminal)

**macOS with Python 3.14 system python (this Mac's setup):**
```bash
# Must prepend the IDF Python 3.9 venv — system python3 (3.14) breaks export.sh
export PATH="$HOME/.espressif/python_env/idf5.3_py3.9_env/bin:$PATH"
. ~/esp/esp-idf/export.sh
```

**Standard (if system python is 3.9–3.12):**
```bash
. ~/esp/esp-idf/export.sh
```

### Set target (one-time per checkout)

```bash
cd firmware/
idf.py set-target esp32s3
```

---

## Step 2 — Pre-Build Configuration

Edit these files **before** building. Never edit constants directly in `main/`.

### 2a. WiFi Credentials (`firmware/config/wifi_config.h`)

```bash
cp firmware/config/wifi_config.h.template firmware/config/wifi_config.h
```

Edit `wifi_config.h`:

```cpp
#define WIFI_SSID          "YOUR_2_4GHz_SSID"   // 2.4 GHz only — no 5 GHz
#define WIFI_PASSWORD      "YOUR_WIFI_PASSWORD"
#define OTA_SERVER_BASE_URL "https://github.com/your-org/CAREC-Project/releases/download/latest/"
```

> `wifi_config.h` is **gitignored** and will never be committed.

---

### 2b. Feature Flags (`firmware/config/build_flags.h`)

```cpp
#define FEATURE_MOTION_GATE   1  // 0 = run detection every loop (skip optical-flow gate)
#define FEATURE_BLE_LOGGING   1  // 0 = disable BLE (~40 KB flash savings)
#define FEATURE_WIFI_OTA      1  // 0 = skip WiFi init (faster boot)
#define FEATURE_DISPLAY       1  // 0 = headless (display_update() is a no-op)

#define DEBUG_VERBOSE         1  // 0 = quiet serial (production)
#define DEBUG_BBOX_RAW        0  // 1 = print raw bounding-box coords (calibration only)
```

**Test modes** (mutually exclusive — an `#error` guard prevents enabling more than one):

```cpp
// Uncomment ONE for hardware bring-up. Leave all commented for full CAREC firmware.
// #define LED_TEST          // WS2813 25-color cycle on GPIO 40 (2 s on / 58 s off)
// #define SPEAKER_TEST      // 1500 Hz heartbeat beep once per LED-OFF transition
// #define DISPLAY_TEST      // SPD2010 RED→YELLOW→GREEN cycle (currently stays dark)
```

---

### 2c. Zone Thresholds (`firmware/config/zone_config.h`)

```cpp
#define DIST_RED     60     // 0–60 cm  → RED zone  (BEEP_CRITICAL)
#define DIST_YELLOW 100     // 60–100 cm → YELLOW zone (BEEP_WARNING); 100+ → GREEN

#define CALIB_BBOX_AT_RED     0.241f   // bbox_w at 60 cm — update after field calibration
#define CALIB_BBOX_AT_YELLOW  0.144f   // bbox_w at 100 cm — update after field calibration
```

---

## Step 3 — Build

```bash
cd firmware/
idf.py build
```

Expected output (first build takes 3–5 min; incremental is ~20 s):
```
...
[100%] Linking CXX executable carec.elf
Generated /path/to/firmware/build/carec.bin
```

**If build fails:**
- `wifi_config.h: No such file` → copy the template (Step 2a)
- Component fetch errors → run `idf.py update-dependencies` then retry
- `set-target` errors → run `idf.py set-target esp32s3` then `idf.py build`

---

## Step 4 — Flash

### Before flashing

Connect SenseCAP Watcher via **USB-C data cable** (not charge-only). Two ports appear:

```bash
ls /dev/cu.usbmodem*
# Higher-numbered port → ESP32-S3 (flash here)
# Lower-numbered port  → Himax WE2 console (read-only)
```

If no port appears: hold the **BOOT button** while plugging in, then release.

### Flash + monitor

```bash
idf.py -p /dev/cu.usbmodem5B142665543 flash monitor
```

Or use the convenience script:

```bash
./firmware/tools/flash.sh                           # auto-detect port
./firmware/tools/flash.sh /dev/cu.usbmodem5B142665543  # specific port
```

Flash progress:
```
Connecting...
Chip is ESP32-S3 (revision v0.2)
Compressed 1,234,567 bytes to 678,901...
Writing at 0x00010000... (100 %)
Hash of data verified.
Hard resetting via RTS pin...
```

### OTA (over-the-air — no USB needed)

OTA fires automatically when:
1. Device has WiFi connectivity
2. Zone is `ZONE_GREEN` **or** wheelchair is `STATIONARY`
3. New firmware binary is available at `OTA_SERVER_BASE_URL`

To push a local OTA build:
```bash
# Host the binary on a local HTTP server
python3 -m http.server 8080 --directory firmware/build/

# Set OTA_SERVER_BASE_URL to http://192.168.x.x:8080/
# Reboot Watcher — it pulls on next WiFi connect
```

> OTA is intentionally blocked in YELLOW and RED zones — the device never reboots mid-alert.

---

## Step 5 — Monitor

```bash
idf.py -p /dev/cu.usbmodem5B142665543 monitor
# Ctrl+] to exit
```

Or with log capture:
```bash
./firmware/tools/monitor.sh   # saves to logs/serial_TIMESTAMP.log
```

### Expected serial output

**Boot:**
```
I (342) CAREC: starting...
I (1203) wifi: Connected to MySSID, IP 192.168.1.42
I (1245) ble: GATT server started
I (1246) CAREC: ready.
```

**Running (obstacle at ~80 cm, wheelchair moving forward):**
```
I (12456) safety: nearest obstacle 78.3 cm  motion=FWD
I (12457) safety: zone YELLOW (60-100 cm)
I (12458) ble: {"zone":"YELLOW","dist_cm":78.3,"motion":"FORWARD","ts_ms":12456}
```

**Motion gate active (wheelchair stationary):**
```
I (8000) motion: STATIONARY — skipping detection
```

---

## Step 6 — Calibration

The distance estimator uses a bounding-box heuristic that needs calibration against the real camera on the real wheelchair.

```bash
# 1. Enable raw bbox logging
#    In firmware/config/build_flags.h: set DEBUG_BBOX_RAW 1
#    Rebuild + flash

# 2. Run the calibration tool
python3 tools/calibration/calibrate_distance.py --port /dev/cu.usbmodem5B142665543

# Follow prompts: hold a 30 cm wide object at 60 cm, then at 100 cm
# Tool outputs CALIB_BBOX_AT_RED and CALIB_BBOX_AT_YELLOW values

# 3. Paste the output into firmware/config/zone_config.h
# 4. Set DEBUG_BBOX_RAW 0
# 5. Rebuild + flash
```

---

## Step 7 — Run Tests

```bash
pip install pytest
pytest tests/ -v
```

Tests verify zone classification logic, distance thresholds, and the 50-scenario obstacle matrix against `DIST_RED=60` and `DIST_YELLOW=100`.

---

## Troubleshooting

| Problem | Likely cause | Fix |
|---------|-------------|-----|
| `wifi_config.h: No such file` | Template not copied | `cp firmware/config/wifi_config.h.template firmware/config/wifi_config.h` |
| Port not found | Charge-only cable or wrong port | Use data-capable USB-C; check higher-numbered port is ESP32-S3 |
| `Connecting...` hangs | Boot mode not entered | Hold BOOT button while plugging in, then release |
| `set-target` error on build | Target not set | `idf.py set-target esp32s3` then retry |
| Component fetch errors | Missing managed_components | `idf.py update-dependencies` |
| Build uses wrong Python | system python3 is 3.14 | Prepend IDF venv: `export PATH="$HOME/.espressif/python_env/idf5.3_py3.9_env/bin:$PATH"` |
| `CAREC ready.` but no detections | SSCMA model not loaded | Pair with SenseCraft Mate app → load object detection model |
| WiFi keeps failing | Wrong SSID/password or 5 GHz | Edit `wifi_config.h` — Watcher is **2.4 GHz only** |
| OTA never fires | Zone not GREEN | Move camera away from obstacles + wait for GREEN |
| Speaker silent | ES8311 init issue | Use `SPEAKER_TEST` mode to verify hardware; check `directional_beep_patterns.h` init |
| LCD completely dark | SPD2010 hardware issue | **Known issue** — display work deferred. Use WS2813 LED (GPIO 40) for visual feedback. |

---

## Related Documents

| Document | What it covers |
|----------|---------------|
| [`firmware/SETUP.md`](../../firmware/SETUP.md) | Detailed IDF install + Python venv workaround |
| [`firmware/README.md`](../../firmware/README.md) | Architecture, safety logic, hardware reference, pin map |
| [`firmware/RGB_LED_BRINGUP.md`](../../firmware/RGB_LED_BRINGUP.md) | Standalone WS2813 LED bring-up test |
| [`firmware/main/idf_component.yml`](../../firmware/main/idf_component.yml) | Component dependencies + versions |
| [`firmware/config/build_flags.h`](../../firmware/config/build_flags.h) | All compile-time feature toggles + test mode selectors |
| [`firmware/config/zone_config.h`](../../firmware/config/zone_config.h) | Distance thresholds + calibration constants |
| [`firmware/config/wifi_config.h.template`](../../firmware/config/wifi_config.h.template) | WiFi + OTA URL template |
| [`docs/api/firmware_api.md`](../api/firmware_api.md) | Public API of every `.h` module |
| [`docs/guides/quick_reference.md`](quick_reference.md) | 30-day checklist |
| [`tools/calibration/calibrate_distance.py`](../../tools/calibration/calibrate_distance.py) | Interactive distance calibration |
| [`tools/log_parser/parse_serial_log.py`](../../tools/log_parser/parse_serial_log.py) | Analyse captured serial logs |
| [`tests/obstacle_test.py`](../../tests/obstacle_test.py) | Zone classification test suite |
| [`.github/workflows/build.yml`](../../.github/workflows/build.yml) | CI auto-build on push |

### Upstream Seeed References

| Resource | URL |
|----------|-----|
| Firmware BSP source | https://github.com/Seeed-Studio/SenseCAP-Watcher-Firmware |
| OSHW hardware (schematics, BOM) | https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher |
| Software Framework | https://wiki.seeedstudio.com/watcher_software_framework/ |
| Function Module Dev Guide | https://wiki.seeedstudio.com/watcher_function_module_development_guide/ |
| UI Integration Guide | https://wiki.seeedstudio.com/watcher_ui_integration_guide/ |
| SenseCraft App | https://wiki.seeedstudio.com/sensecap_app_introduction/ |
| Service Framework | https://wiki.seeedstudio.com/watcher_software_service_framework/ |
| Local AI Deployment | https://wiki.seeedstudio.com/watcher_local_deploy/ |
