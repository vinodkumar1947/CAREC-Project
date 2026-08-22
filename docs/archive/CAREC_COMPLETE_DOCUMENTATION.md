# CAREC — Complete Project Documentation
## Collision Avoidance with Real-time Edge Computing

> **Single-file reference for the entire CAREC project.**  
> Everything you need to understand, build, deploy, and maintain CAREC is in this document.

| Field | Detail |
|-------|--------|
| **Project** | CAREC — Smart Wheelchair Safety System |
| **Owner** | Vinod Kumar (12+ years embedded firmware / IoT) |
| **Target user** | 6-year-old child, Numotion electric wheelchair |
| **Hardware** | SenseCAP Watcher W1-A Clear Enclosure — $59.99 |
| **Status** | Hardware received May 4, 2026 → Week 1 development |
| **Version** | 0.1.1 — May 5, 2026 |

---

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [Problem Statement](#2-problem-statement)
3. [Hardware](#3-hardware)
4. [System Architecture](#4-system-architecture)
5. [Firmware Architecture](#5-firmware-architecture)
6. [Detection Pipeline](#6-detection-pipeline)
7. [Alert Zones](#7-alert-zones)
8. [Configuration Files](#8-configuration-files)
9. [Programming & Flashing](#9-programming--flashing)
10. [Calibration](#10-calibration)
11. [BLE Caregiver Notifications](#11-ble-caregiver-notifications)
12. [WiFi & OTA Updates](#12-wifi--ota-updates)
13. [Mounting Guide](#13-mounting-guide)
14. [Testing Protocol](#14-testing-protocol)
15. [Safety](#15-safety)
16. [Development Roadmap](#16-development-roadmap)
17. [Market Comparison](#17-market-comparison)
18. [Research Publication](#18-research-publication)
19. [Project Structure](#19-project-structure)
20. [Support & Resources](#20-support--resources)

---

## 1. Project Overview

**CAREC** (Collision Avoidance with Real-time Edge Computing) is an open-source, non-invasive AI obstacle detection system for electric wheelchairs. It clips onto the armrest in 5 minutes, needs no wheelchair modification, and gives the child and caregiver real-time audio + visual + mobile alerts when an obstacle is within dangerous range.

### What It Does

```
Camera sees obstacle → NPU classifies it → distance estimated →
zone assigned (RED / YELLOW / GREEN) →
speaker beeps + display flashes + caregiver phone notified
```

### What It Does NOT Do

- Stop the wheelchair automatically (warning aid only)
- Detect obstacles behind or to the sides (forward-facing only — Phase 2 adds rear)
- Work outdoors in bright sunlight (designed for indoor use)
- Replace caregiver supervision

### Key Numbers

| Metric | Value |
|--------|-------|
| Total system cost | ~$93–112 |
| Detection latency | < 200 ms |
| Detection accuracy | target ≥ 85% |
| False positive rate | target ≤ 5% |
| Battery runtime | 50+ hours (10,000 mAh LiPo) |
| RED zone threshold | 60 cm |
| YELLOW zone threshold | 100 cm |
| Safety loop rate | 10 Hz (100 ms) |
| Camera FOV | 120° horizontal |

---

## 2. Problem Statement

### The Need

A 6-year-old driving an electric wheelchair at even 0.5 m/s will close 60 cm in 1.2 seconds. At that speed, reaction time to stop the chair is 1–2 seconds. Without a warning system, collisions with furniture, walls, people, and door frames are frequent.

### The Gap

| Solution | Cost | Problem |
|----------|------|---------|
| Braze Mobility Sentina Plus | $4,040 | Rear-only, no WiFi, no app, no AI, no caregiver alerts |
| Strutt EV1 | $8,000–15,000 | Semi-autonomous wheelchair, not a retrofit, adult-focused |
| Academic AI wheelchair | N/A | Research prototype, not deployable |
| Custom DIY (ESP32 + sensors) | $100–200 | Untested, no speaker/display, complex wiring |

### The CAREC Solution

One device. $100. No wiring. Open-source. App-connected. OTA-updatable.

---

## 3. Hardware

### 3.1 Primary Device — SenseCAP Watcher W1-A Clear Enclosure

**"The Physical AI Agent for Smarter Spaces"**  
Seeed Studio · SKU 113991315 · $59.99  
Received: May 4, 2026 · Firmware: ESP 1.1.7 · AI NPU: 2024.08.16

| Component | Specification |
|-----------|--------------|
| **Processor** | ESP32-S3, dual-core Xtensa LX7, 240 MHz |
| **Memory** | 8 MB PSRAM, 32 MB Flash |
| **AI Accelerator** | Himax WiseEye2 HX6538 — ARM Cortex-M55 + Ethos-U55 NPU |
| **NPU Flash** | 16 MB (AI model storage) |
| **Camera** | 5MP OV5647, 120° FOV, fixed focus 3 m |
| **Display** | 1.45" touch screen, 412×412 px |
| **Speaker** | Built-in, 1W output |
| **Microphone** | Built-in, omnidirectional, 3 m pickup radius |
| **WiFi** | 2.4 GHz IEEE 802.11 b/g/n, ~100 m range |
| **Bluetooth** | BT 5, ~30 m range |
| **USB** | USB-C — power input + programming |
| **Onboard battery** | Li-ion 3.7 V, 400 mAh (~6 hr standalone) |
| **Power input** | 5 V DC via USB-C, 150–200 mA average |
| **Expansion** | 1× Grove IIC + 2×4 GPIO pin headers |
| **Storage** | Micro-SD slot, up to 32 GB FAT32 |
| **Enclosure** | Transparent clear plastic, 69×65×20 mm, ~100 g |
| **Operating temp** | 0°C to 45°C |
| **Integrations** | SenseCraft, Home Assistant, Node-RED, XiaoZhi |

### 3.2 Bill of Materials — Phase 1

| Item | Spec | Price | Status |
|------|------|-------|--------|
| SenseCAP Watcher W1-A Clear | SKU 113991315 | $59.99 | ✅ Received |
| LiPo Battery | 10,000 mAh, 3.7 V, USB-C output | $15–25 | 🔄 Ordered |
| Ball Joint Mount | 1/4" thread, 360° + ±80° tilt | $10–15 | 🔄 Ordered |
| Tube Clamp | 1–1.5" adjustable, stainless steel | $8–12 | 🔄 Ordered |
| Micro-SD Card (optional) | 32 GB, Class 10 | $8–15 | optional |
| **Total (excl. SD)** | | **~$93–112** | |

### 3.3 Phase 2 Expansion (via Grove IIC — no soldering)

| Sensor | Purpose | Cost |
|--------|---------|------|
| Grove ADXL345 Accelerometer | Replace optical-flow motion gate — higher reliability | ~$8 |
| Grove HC-SR04 Ultrasonic | Rear obstacle detection (360° coverage) | ~$6 |
| Grove IR Cliff Sensor | Drop-off / step-down detection | ~$7 |

### 3.4 Hardware Evolution (Why This Device)

| Phase | Hardware Considered | Decision |
|-------|---------------------|---------|
| 1 | ESP32-C6 + HC-SR04 + Sharp IR sensors | ❌ Deprecated — too many sensors, unreliable |
| 2 | XIAO ESP32-S3 Sense + XIAO Vision AI | ❌ Rejected — no speaker, no enclosure, limited |
| 3 | **SenseCAP Watcher W1-A** | ✅ **FINAL** — all-in-one, open-source, best ratio |

---

## 4. System Architecture

```
┌──────────────────────────────────────────────────────────┐
│                 CAREC SYSTEM OVERVIEW                    │
└──────────────────────────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────┐
  │         SenseCAP Watcher W1-A (on wheelchair)       │
  │                                                     │
  │  PERCEPTION                                         │
  │  OV5647 Camera (5MP, 120° FOV) ──► Himax HX6538    │
  │                                      NPU Inference  │
  │                                      (~80–150 ms)   │
  │  PROCESSING (ESP32-S3)                              │
  │  ├─ Optical-flow motion gate                        │
  │  ├─ Bounding-box distance heuristic                 │
  │  ├─ 3-zone classifier (RED/YELLOW/GREEN)            │
  │  └─ Non-blocking alert state machines               │
  │                                                     │
  │  OUTPUT                                             │
  │  ├─ 1W Speaker → beep patterns                      │
  │  ├─ 1.45" Display → colour + blink                  │
  │  ├─ BLE 5 → caregiver phone                         │
  │  └─ WiFi → OTA firmware updates                     │
  └─────────────────────────────────────────────────────┘
              │ USB-C
  ┌─────────────────────────────────────────────────────┐
  │  10,000 mAh LiPo battery (50+ hr runtime)           │
  └─────────────────────────────────────────────────────┘
              │ Ball Joint (1/4") + Tube Clamp
  ┌─────────────────────────────────────────────────────┐
  │  Wheelchair Armrest (Numotion) — no modification     │
  └─────────────────────────────────────────────────────┘
              │ BLE 5 (~30 m)
  ┌─────────────────────────────────────────────────────┐
  │  Caregiver phone — SenseCraft Mate app              │
  │  (iOS / Android — free)                             │
  └─────────────────────────────────────────────────────┘
```

### Software Layer Stack

```
┌────────────────────────────────────────────┐
│  CAREC Application (CAREC_obstacle_        │  ← Your code
│  detection.ino + 7 header modules)         │
├────────────────────────────────────────────┤
│  firmware/config/                          │  ← Your config
│  zone_config.h │ build_flags.h │ wifi.h   │
├────────────────────────────────────────────┤
│  SenseCraft SDK (Seeed Studio)             │  ← NPU + display
├────────────────────────────────────────────┤
│  ESP-IDF v5.x / Arduino ESP32 Core        │  ← Espressif
├────────────────────────────────────────────┤
│  ESP32-S3 Hardware + Himax HX6538 NPU      │  ← Silicon
└────────────────────────────────────────────┘
```

---

## 5. Firmware Architecture

### 5.1 Source Files

```
firmware/
├── src/
│   ├── CAREC_obstacle_detection.ino    ← MAIN ENTRY POINT
│   ├── sensecraft_detection.h          ← NPU inference (stub → real SDK)
│   ├── distance_estimator.h            ← bounding-box → cm distance
│   ├── motion_detector.h               ← optical-flow gate (fwd/bwd/stationary)
│   ├── directional_beep_patterns.h     ← non-blocking beep state machine
│   ├── display_alert.h                 ← 1.45" display RED/YELLOW/GREEN driver
│   ├── ble_logger.h                    ← BLE GATT event logger (stub → GATT)
│   ├── wifi_ota.h                      ← WiFi connect + OTA update
│   └── local_llm_integration.cpp       ← Ollama LLM (Phase 2, inactive)
│
├── config/
│   ├── wifi_config.h.template          ← Template — copy and fill in credentials
│   ├── wifi_config.h                   ← Your credentials (gitignored)
│   ├── zone_config.h                   ← DIST_RED=60, DIST_YELLOW=100, calibration
│   └── build_flags.h                   ← Feature on/off toggles
│
└── tools/
    ├── flash.sh                        ← Compile + upload script
    └── monitor.sh                      ← Serial monitor with auto-log
```

### 5.2 Main Loop (100 ms / 10 Hz)

```cpp
void loop() {
    // 1. Motion gate — skip all alerts when wheelchair is stationary
    MotionState motion = motion_update();
    if (motion == MOTION_STATIONARY) {
        display_set_zone(ZONE_GREEN);  display_update();
        beep_trigger(BEEP_NONE);       beep_update();
        ota_set_safe(true);            ota_check_and_update();
        delay(100);
        return;  // ← exits here when not moving
    }

    // 2. NPU object detection (80–150 ms inference on Himax HX6538)
    DetectionResult result = sensecraft_detect();

    // 3. Distance estimation (bounding-box heuristic)
    float distance_cm = nearest_obstacle_cm(&result);

    // 4. Zone classification
    int zone = (distance_cm < DIST_RED)    ? ZONE_RED    :
               (distance_cm < DIST_YELLOW) ? ZONE_YELLOW : ZONE_GREEN;

    // 5. Speaker (non-blocking state machine)
    beep_trigger(zone == ZONE_RED ? BEEP_CRITICAL : zone == ZONE_YELLOW ? BEEP_WARNING : BEEP_NONE);
    beep_update();

    // 6. Display (non-blocking state machine)
    display_set_zone(zone);
    display_update();

    // 7. BLE event to SenseCraft Mate
    ble_log_event(distance_cm, zone, motion);

    // 8. OTA — only when GREEN (no obstacle threat)
    ota_set_safe(zone == ZONE_GREEN);
    ota_check_and_update();

    delay(100);
}
```

### 5.3 Module API Summary

| Module | Key functions | Status |
|--------|--------------|--------|
| `sensecraft_detection.h` | `sensecraft_init()` · `sensecraft_detect()` | 🟡 Stub |
| `distance_estimator.h` | `nearest_obstacle_cm(&result)` | ✅ Done |
| `motion_detector.h` | `motion_init()` · `motion_update()` · `motion_log()` | 🟡 Stub |
| `directional_beep_patterns.h` | `beep_trigger(pattern)` · `beep_update()` | 🟡 Stub |
| `display_alert.h` | `display_init()` · `display_set_zone(zone)` · `display_update()` | 🟡 Stub |
| `ble_logger.h` | `ble_logger_init()` · `ble_log_event(dist, zone, motion)` | 🟡 Serial mirror |
| `wifi_ota.h` | `ota_set_safe(bool)` · `ota_check_and_update()` | 🟡 Stub |

> 🟡 Stub = compiles and runs, but real hardware calls need to replace TODO bodies. See Week 2 plan.

Full API reference → [`docs/api/firmware_api.md`](api/firmware_api.md)

---

## 6. Detection Pipeline

```
OV5647 Camera — 5MP, 120° FOV, 10 fps
        │
        ▼
Optical-flow Motion Gate (motion_detector.h)
  80×60 grayscale thumbnail
  12 Lucas-Kanade corner features tracked
  ┌─ STATIONARY → GREEN display + silence → OTA allowed → SKIP
  └─ FORWARD / BACKWARD → continue ↓
        │
        ▼
SenseCraft NPU Inference (sensecraft_detection.h)
  Himax HX6538 — ARM Cortex-M55 + Ethos-U55
  On-device, no cloud, 80–150 ms per frame
  Returns: bounding boxes + labels + confidence scores
        │
        ▼
Distance Estimation (distance_estimator.h)
  Formula: distance_cm = real_width_cm / (bbox_w_norm × 2 × tan(60°))
  Calibrated: bbox_w=0.241 → 60 cm · bbox_w=0.144 → 100 cm
  Selects nearest (widest bbox) detection above confidence threshold
        │
        ▼
Zone Classification
  distance_cm < 60   → ZONE_RED
  distance_cm < 100  → ZONE_YELLOW
  distance_cm ≥ 100  → ZONE_GREEN
        │
        ├──► Speaker: BEEP_CRITICAL / BEEP_WARNING / BEEP_NONE
        ├──► Display: RED blink / YELLOW blink / GREEN solid
        └──► BLE: JSON event to SenseCraft Mate caregiver app

Total end-to-end latency: < 200 ms
```

---

## 7. Alert Zones

### Zone Definitions

| Zone | Distance | Display | Blink Period | Audio Pattern | BLE Event |
|------|----------|---------|-------------|--------------|-----------|
| **GREEN** | > 100 cm | Solid green | — | Silent | None |
| **YELLOW** | 60–100 cm | Slow yellow blink | 600 ms | 1 × 300 ms beep / sec | Warning JSON |
| **RED** | < 60 cm | Fast red blink | 150 ms | 3 × 80 ms burst, repeat | Critical JSON |

### Beep Pattern Detail

```
BEEP_NONE     : silence

BEEP_WARNING  : ─────╮    ╭──
                 OFF  │ON  │ OFF ...
                      300ms  700ms  (repeating, 1 Hz)

BEEP_CRITICAL : ─╮ ╭╮ ╭╮ ╭─────╮ ╭╮ ╭╮ ╭─
                  │ ││ ││ │     │ ││ ││ │
                  80 40 80 40 80ms  200ms gap (repeating)
```

### Design Rationale

- **60 cm RED threshold:** pediatric wheelchair at 0.5 m/s closes 60 cm in 1.2 s — enough warning time to release joystick
- **100 cm YELLOW threshold:** pre-warning zone — caregiver and child can start reacting
- **Motion gate:** eliminates false positives when wheelchair is parked — most common source of alert fatigue

---

## 8. Configuration Files

All configuration is in `firmware/config/` — edit these, not the source files.

### 8.1 `wifi_config.h` (gitignored — create from template)

```bash
cp firmware/config/wifi_config.h.template firmware/config/wifi_config.h
```

```cpp
#define WIFI_SSID      "YOUR_2_4GHz_SSID"      // 2.4 GHz ONLY — no 5 GHz
#define WIFI_PASSWORD  "YOUR_WIFI_PASSWORD"
#define OTA_SERVER_BASE_URL  "https://github.com/yourusername/CAREC-Project/releases/download/latest/"
```

### 8.2 `zone_config.h` — Distance Thresholds

```cpp
#define DIST_RED     60     // 0–60 cm → RED zone
#define DIST_YELLOW 100     // 60–100 cm → YELLOW; 100+ → GREEN

#define CALIB_BBOX_AT_RED     0.241f   // bbox_w_norm at 60 cm
#define CALIB_BBOX_AT_YELLOW  0.144f   // bbox_w_norm at 100 cm
#define CALIB_HALF_FOV_DEG   60.0f     // OV5647: 120°/2
#define CALIB_REF_WIDTH_CM   30.0f     // reference object width for calibration
```

### 8.3 `build_flags.h` — Feature Toggles

```cpp
#define FEATURE_MOTION_GATE     1   // 0 = skip motion gate (always detect)
#define FEATURE_BLE_LOGGING     1   // 0 = disable BLE (~40 KB flash savings)
#define FEATURE_WIFI_OTA        1   // 0 = skip WiFi init (faster boot)
#define FEATURE_DISPLAY         1   // 0 = headless mode

#define DEBUG_VERBOSE           1   // 0 = quiet serial (production)
#define DEBUG_BBOX_RAW          0   // 1 = print raw bbox data (calibration only)

// Phase 2 (disabled until hardware connected)
#define FEATURE_ACCEL_GATE      0   // Grove ADXL345 motion gate
#define FEATURE_LOCAL_LLM       0   // Ollama LLM scene description
#define FEATURE_SD_LOGGING      0   // Micro-SD event log
#define FEATURE_REAR_ULTRASONIC 0   // Grove HC-SR04 rear sensor
```

---

## 9. Programming & Flashing

### Quick Flash (arduino-cli)

```bash
# 1. Create WiFi config
cp firmware/config/wifi_config.h.template firmware/config/wifi_config.h
# edit firmware/config/wifi_config.h

# 2. Flash
./firmware/tools/flash.sh                        # auto-detect port
./firmware/tools/flash.sh /dev/cu.usbmodemXXXX  # specific port

# 3. Monitor
./firmware/tools/monitor.sh   # 115200 baud, logs saved to logs/
```

### Arduino IDE

```
1. Install Arduino IDE v2.x — https://www.arduino.cc/en/software
2. Preferences → Additional boards URLs:
   https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json
3. Boards Manager → search "Seeed XIAO" → Install
4. Tools → Board → XIAO ESP32S3
5. File → Open → firmware/src/CAREC_obstacle_detection.ino
6. Sketch → Verify/Compile (Ctrl+R)
7. Tools → Port → /dev/cu.usbmodemXXXX
8. Sketch → Upload (Ctrl+U)
```

### Expected Serial Output on Boot

```
CAREC starting...
[WiFi] Connecting to MySSID.......
[WiFi] Connected: 192.168.1.42
[BLE] Init OK (stub — Serial mirror until GATT wired)
CAREC ready.
```

### Troubleshooting Flash

| Problem | Fix |
|---------|-----|
| `wifi_config.h not found` | `cp firmware/config/wifi_config.h.template firmware/config/wifi_config.h` |
| Port not found | Use data-capable USB-C cable (not charge-only) |
| Upload fails at "Connecting..." | Hold BOOT button while inserting USB-C, release |
| Serial garbage | Set baud rate to 115200 |
| "Arduino.h not found" in editor | IntelliSense false positive — compiles fine |

Full guide → [`docs/guides/programming_guide.md`](guides/programming_guide.md)

---

## 10. Calibration

After hardware arrives, calibrate the distance estimator against the real camera.

```bash
# 1. Enable raw bbox output
# Edit firmware/config/build_flags.h: set DEBUG_BBOX_RAW 1
# Recompile + flash

# 2. Run calibration tool
python3 tools/calibration/calibrate_distance.py --port /dev/cu.usbmodemXXXX
# Follow prompts: hold 30 cm wide object at 60 cm, then 100 cm

# 3. Tool outputs:
# #define CALIB_BBOX_AT_RED     0.241f
# #define CALIB_BBOX_AT_YELLOW  0.144f

# 4. Paste into firmware/config/zone_config.h
# 5. Set DEBUG_BBOX_RAW 0, recompile + flash
# 6. Rerun tests: pytest tests/obstacle_test.py
```

### Distance Formula

```
distance_cm = CALIB_REF_WIDTH_CM / (bbox_w_norm × 2 × tan(CALIB_HALF_FOV_DEG))

Where:
  CALIB_REF_WIDTH_CM  = 30.0   (physical width of reference object, cm)
  bbox_w_norm         = 0.0–1.0 (normalised bounding box width from NPU)
  CALIB_HALF_FOV_DEG  = 60.0   (OV5647: 120° total / 2)
```

---

## 11. BLE Caregiver Notifications

### Event Format

Every obstacle detection sends a JSON string via BLE GATT notify:

```json
{"zone":"RED","dist_cm":42.3,"motion":"FORWARD","ts_ms":12345678}
```

| Field | Values |
|-------|--------|
| `zone` | `"GREEN"` · `"YELLOW"` · `"RED"` |
| `dist_cm` | float, distance to nearest obstacle in cm |
| `motion` | `"STATIONARY"` · `"FORWARD"` · `"BACKWARD"` |
| `ts_ms` | `millis()` timestamp since device boot |

### GATT Service (Nordic UART)

| Role | UUID |
|------|------|
| Service | `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` |
| TX Characteristic (notify) | `6E400003-B5A3-F393-E0A9-E50E24DCCA9E` |

> **Current status:** BLE stub mirrors all events to `Serial.printf()`. Replace `_ble_notify()` in `ble_logger.h` with real `BLECharacteristic::setValue() + notify()` once Seeed confirms the SenseCraft Mate UUIDs.

### SenseCraft Mate App

| Platform | Link |
|----------|------|
| iOS | https://apps.apple.com/us/app/sensecraft/id1619944834 |
| Android | https://play.google.com/store/apps/details?id=cc.seeed.sensecapmate |

Pairing steps → [`mobile/README.md`](../mobile/README.md)

---

## 12. WiFi & OTA Updates

### WiFi Setup

```cpp
// In firmware/config/wifi_config.h (gitignored):
#define WIFI_SSID      "YOUR_SSID"    // 2.4 GHz ONLY
#define WIFI_PASSWORD  "YOUR_PASSWORD"
```

CAREC connects to WiFi on boot. If disconnected, it retries automatically. Detection continues even when WiFi is offline — WiFi is only needed for OTA.

### OTA Safety Gate

OTA updates are **intentionally blocked** in YELLOW and RED zones:

```cpp
ota_set_safe(zone == ZONE_GREEN);   // only allow when path is clear
ota_check_and_update();              // no-op if not safe
```

This prevents the device from rebooting mid-alert — a critical safety requirement.

### OTA Flow

```
1. Device connects to WiFi
2. ota_check_and_update() polls OTA_SERVER_BASE_URL/manifest.json
3. If new version AND zone == GREEN → download firmware binary
4. Verify SHA-256 hash
5. Flash to OTA partition
6. Reboot into new firmware
7. If boot fails → automatic rollback to previous version
```

---

## 13. Mounting Guide

### Parts

- SenseCAP Watcher W1-A
- Ball joint (1/4" thread)
- Tube clamp (1–1.5" stainless, adjustable)
- USB-C cable (1 m, right-angle preferred)
- 3× velcro cable ties

### Steps

```
Step 1: Thread ball joint into Watcher 1/4" mount point (hand-tight)
Step 2: Slide tube clamp around wheelchair armrest
Step 3: Attach ball joint base to clamp bracket
Step 4: Aim camera: forward-facing, ~45° downward tilt
        → SenseCraft Mate Live Camera: floor at 80 cm in lower third of frame
Step 5: Lock ball joint locking knob firmly
Step 6: Route USB-C along inner armrest → velcro tie every 20 cm
Step 7: Connect to 10,000 mAh battery (in lap bag or armrest pocket)
Step 8: SHAKE TEST — Watcher must not shift under firm grip
Step 9: Fully tighten clamp screw
```

**Install time:** 5 min · **Remove time:** 2 min · **Wheelchair damage:** Zero

Full guide → [`docs/guides/mounting_guide.md`](guides/mounting_guide.md)

---

## 14. Testing Protocol

### Pre-Child-Use Test Matrix

| Test | Method | Pass Criteria |
|------|--------|--------------|
| Detection accuracy | 50 obstacle scenarios (varied objects + distances) | ≥ 85% |
| False positive rate | 30-min session, empty room, wheelchair stationary | ≤ 5% |
| Latency | Obstacle placed → first beep, measured 10× | ≤ 200 ms |
| Battery runtime | Continuous from full charge | ≥ 8 hours |
| Speaker audibility | Measure at 2 m with wheelchair motor running | Clearly audible |
| BLE alert delivery | Trigger 10 RED events, count received on phone | 100% |
| OTA update | Deploy test firmware, verify installed | Clean install |
| Mount stability | Shake test — hold armrest firmly and jostle | No wobble |

### 50-Obstacle Scenario Matrix

Test each object at 30 cm (RED), 80 cm (YELLOW), 120 cm (GREEN):

- Chair leg (thin)
- Table leg (wide)
- Wall (flat surface)
- Person standing
- Person crouching
- Backpack on floor
- Door frame
- Cardboard box
- Pet / dog
- Step / threshold

Results CSV → `tests/results/obstacle_test_results.csv`

### Running Automated Tests

```bash
pip install pytest
pytest tests/obstacle_test.py -v
```

Tests verify: zone classification logic, distance thresholds, beep pattern mapping.

---

## 15. Safety

### Pre-Use Checklist (before every session)

- [ ] Clear enclosure unobstructed — no smudges/tape on camera lens
- [ ] Tube clamp tight — Watcher does not shift when armrest shaken
- [ ] USB-C cable away from wheels and moving parts
- [ ] Battery ≥ 20% charge
- [ ] Hand at 30 cm → RED display + triple beep within 200 ms
- [ ] Remove hand → GREEN display within 200 ms

### Zone Reference Card (for caregiver)

| Colour | Sound | Meaning | Do |
|--------|-------|---------|-----|
| **GREEN** (solid) | None | Clear path | Normal |
| **YELLOW** (blink) | Single beep | 60–100 cm ahead | Caution, slow |
| **RED** (fast blink) | Triple burst | < 60 cm — imminent | STOP |

### Critical Safety Rule

> **CAREC is a warning aid, not a safety stop.** It does not control the wheelchair. Caregiver supervision is required at all times during child use.

### Failure Mode Summary

| Failure | Mitigated by |
|---------|-------------|
| False negative (misses obstacle) | Pre-use 30 cm hand test · caregiver backup |
| Constant false alarm | Motion gate eliminates stationary alerts |
| Device falls off | Stainless clamp + pre-use shake test |
| Cable in wheel | Velcro routing on inner armrest side |
| Battery depletes | 50+ hr runtime · pre-use battery check |
| OTA mid-alert | OTA safety gate (GREEN zone only) |

Full FMEA → [`docs/safety/failure_modes.md`](safety/failure_modes.md)  
Pre-use checklist → [`docs/safety/safety_checklist.md`](safety/safety_checklist.md)

---

## 16. Development Roadmap

### Phase 1 — Hardware + Firmware MVP (Weeks 1–4, May 2026)

| Week | Dates | Goal |
|------|-------|------|
| 1 | May 4–10 | ✅ Hardware received · Pair/OTA · Dev environment |
| 2 | May 11–17 | Replace stubs with real SDK calls · Working detection loop |
| 3 | May 18–24 | Calibration · BLE GATT · WiFi OTA |
| 4 | May 25–31 | 50-obstacle test · Battery test · Safety sign-off |

**Milestone:** `CAREC ready.` → real detections → correct beep → display → BLE event

### Phase 2 — Advanced Features (Weeks 5–8, June 2026)

- Local LLM via Ollama + LLaMA-2 (spoken scene description)
- Custom OTA pipeline (GitHub Actions + esp_https_ota)
- Complete BLE GATT implementation
- Home Assistant + Node-RED integration
- Phase 2 Grove sensors (accelerometer, rear ultrasonic, cliff sensor)

### Phase 3 — Hardening (Weeks 9–12, July 2026)

- 72-hour continuous burn-in test
- 100-cycle power cycling stress test
- Full FMEA validation
- Caregiver training + sign-off
- Go/No-Go decision for daily use

### Phase 4 — Daily Use + Maintenance (from Week 12 onwards)

- Daily pre-use checklist
- Weekly 3-point obstacle test
- Monthly performance review
- CAREC v2.0 planning (rear detection, cliff sensor, LLM improvements)

---

## 17. Market Comparison

| Feature | **CAREC** | Braze Mobility | Strutt EV1 | DIY ESP32 |
|---------|-----------|----------------|-----------|-----------|
| **Price** | **~$100** | $1,850–4,040 | $8,000–15,000 | $100–200 |
| **AI camera** | ✅ 5MP NPU | ❌ No camera | ✅ Full AI | ⚠️ Basic |
| **Forward detection** | ✅ 120° FOV | ⚠️ Rear only | ✅ 360° | ⚠️ Varies |
| **Audio alerts** | ✅ Built-in 1W | ✅ Beep | ✅ Yes | ⚠️ DIY |
| **Display** | ✅ 1.45" colour | ⚠️ LED only | ✅ Yes | ❌ None |
| **Mobile app** | ✅ SenseCraft Mate | ❌ None | ⚠️ Basic | ❌ None |
| **WiFi OTA** | ✅ Yes | ❌ No | ⚠️ Unknown | ❌ No |
| **Open source** | ✅ Fully | ❌ No | ❌ No | ✅ Yes |
| **Non-invasive** | ✅ 5 min, no drilling | ⚠️ Wired | ❌ Replaces chair | ⚠️ Varies |
| **Child-focused** | ✅ Age 6+ | ⚠️ All ages | ❌ Adult focus | ❌ No |
| **Cost vs CAREC** | — | **40× more** | **150× more** | Similar |

---

## 18. Research Publication

CAREC is being submitted to **ISICVA 2026** — 2nd International Symposium on Innovations in Computer Vision and Applications (Kolkata, India, October 13–14, 2026). Springer Nature LNNS Series — Scopus Indexed.

| Date | Milestone |
|------|-----------|
| June 15, 2026 | Paper submission deadline |
| July 15, 2026 | Acceptance notification |
| July 31, 2026 | Camera-ready submission |
| Oct 13–14, 2026 | Symposium (Hybrid) |

**Track:** Track 4 — Smart Vision Applications (Assistive technologies)  
**Paper title:** *CAREC: A Real-Time Edge AI Obstacle Detection System for Pediatric Electric Wheelchairs Using On-Device Computer Vision*

Research folder → [`research/ISICVA2026/`](../research/ISICVA2026/README.md)

---

## 19. Project Structure

```
CAREC-Project/
├── README.md                         ← Project landing page
│
├── firmware/
│   ├── src/                          ← All C++ source (edit here)
│   ├── config/                       ← wifi_config.h · zone_config.h · build_flags.h
│   ├── lib/                          ← Library docs + vendored libs
│   ├── tools/                        ← flash.sh · monitor.sh
│   └── releases/                     ← OTA binaries + manifest.json
│
├── hardware/
│   ├── BOM.md                        ← Parts list + costs
│   ├── schematics/watcher_pinout.md  ← GPIO + connector reference
│   ├── datasheets/                   ← Component datasheet links
│   └── photos/                       ← Assembly photos
│
├── docs/
│   ├── CAREC_COMPLETE_DOCUMENTATION.md  ← THIS FILE
│   ├── README.md                     ← Docs index
│   ├── specifications/               ← system_spec · hardware status · Watcher selection
│   ├── guides/                       ← programming · mounting · quick_reference · caregiver
│   ├── api/firmware_api.md           ← Full module API reference
│   ├── safety/                       ← safety_checklist · failure_modes
│   └── business/                     ← competitive_analysis
│
├── tests/
│   ├── obstacle_test.py              ← Main test suite (pytest)
│   ├── conftest.py                   ← Fixtures + classify_zone() helper
│   ├── fixtures/sample_detections.json
│   └── results/                      ← Test run output (gitignored)
│
├── tools/
│   ├── calibration/calibrate_distance.py  ← Interactive distance calibration
│   └── log_parser/parse_serial_log.py     ← Serial log analyser
│
├── mobile/README.md                  ← BLE GATT + SenseCraft Mate guide
├── examples/
│   ├── minimal_detection/            ← NPU-only, no BLE/OTA — bringup sketch
│   └── zone_test/                    ← Hardware test: cycles through all zones
│
├── research/
│   └── ISICVA2026/                   ← Paper submission (deadline Jun 15, 2026)
│       ├── README.md                 ← Conference details + writing timeline
│       ├── paper/                    ← outline · abstract · draft · figures
│       ├── submission/checklist.md   ← Springer LNCS + CMT3 steps
│       └── references/references.bib
│
├── tasks/CAREC_tasks.csv             ← 65 tasks (ClickUp-importable)
├── weekly-reviews/                   ← week-01.md · week-02.md … (fill weekly)
├── .github/workflows/                ← build.yml · test.yml (CI/CD)
└── .gitignore                        ← wifi_config.h · *.bin · logs/ excluded
```

---

## 20. Support & Resources

### Official Seeed Studio

| Resource | Link |
|----------|------|
| Product page | https://www.seeedstudio.com/SenseCAP-Watcher-W1-A-p-5979.html |
| OSHW GitHub (firmware + schematics) | https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher |
| Wiki + setup guide | https://wiki.seeedstudio.com/sensecap_watcher/ |
| Forum | https://forum.seeedstudio.com/ |
| Discord | https://discord.gg/seeed |
| Support | https://www.seeedstudio.com/support |

### Development Tools

| Tool | Link |
|------|------|
| Arduino IDE v2.x | https://www.arduino.cc/en/software |
| ESP-IDF v5.x | https://docs.espressif.com/projects/esp-idf/ |
| arduino-cli | https://arduino.github.io/arduino-cli/ |
| SenseCraft Mate iOS | https://apps.apple.com/us/app/sensecraft/id1619944834 |
| SenseCraft Mate Android | https://play.google.com/store/apps/details?id=cc.seeed.sensecapmate |
| Ollama (local LLM) | https://ollama.ai/ |
| Home Assistant | https://www.home-assistant.io/ |

### Key Documents (internal)

| Document | Purpose |
|----------|---------|
| [`docs/guides/programming_guide.md`](guides/programming_guide.md) | Compile + flash guide |
| [`docs/api/firmware_api.md`](api/firmware_api.md) | Module API reference |
| [`docs/guides/quick_reference.md`](guides/quick_reference.md) | 30-day action checklist |
| [`docs/guides/mounting_guide.md`](guides/mounting_guide.md) | Physical installation |
| [`docs/guides/caregiver_guide.md`](guides/caregiver_guide.md) | Non-technical caregiver guide |
| [`docs/safety/safety_checklist.md`](safety/safety_checklist.md) | Pre-use safety checks |
| [`docs/safety/failure_modes.md`](safety/failure_modes.md) | FMEA — 7 failure modes |
| [`hardware/BOM.md`](../hardware/BOM.md) | Bill of materials |
| [`hardware/schematics/watcher_pinout.md`](../hardware/schematics/watcher_pinout.md) | GPIO reference |
| [`mobile/README.md`](../mobile/README.md) | BLE GATT + SenseCraft Mate |
| [`weekly-reviews/week-01.md`](../weekly-reviews/week-01.md) | Week 1 log |
| [`research/ISICVA2026/README.md`](../research/ISICVA2026/README.md) | Conference submission |

### Contact

**Vinod Kumar** — Project Owner  
12+ years embedded firmware / IoT development  
Email: vinodkumar1947@gmail.com  
GitHub: https://github.com/yourusername/CAREC-Project

---

*Last Updated: May 5, 2026 — Hardware received, Week 1 in progress*  
*Next update: After Week 2 stub replacement complete*
