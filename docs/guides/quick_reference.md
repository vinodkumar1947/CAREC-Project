# CAREC: QUICK REFERENCE & ACTION PLAN
## SenseCAP Watcher W1-A Clear Enclosure Edition

> **Updated May 15, 2026** — Hardware is the **SenseCAP Watcher W1-A Clear Enclosure** ($59.99). Received May 4, 2026. Custom firmware (ESP-IDF v5.3.5) actively running: LED, speaker, SSCMA detection all validated. Main loop ready to flash-validate.
> The old ESP32-C6 multi-sensor design has been deprecated.

---

## PROJECT AT A GLANCE

**System Name:** CAREC (Collision Avoidance with Real-time Edge Computing)  
**Purpose:** Smart safety layer for 6-year-old's electric wheelchair  
**Approach:** External, non-invasive, single-device — plug-and-play  
**Timeline:** MVP in 4 weeks, Full system in 12 weeks  
**Budget:** ~$93–112 total (Phase 1 hardware)  
**Status:** Hardware ordered → Development starts Week 1, May 2026

---

## 🏪 MARKET COMPARISON SUMMARY

| Company | Product | Price | What They Do | Limitations |
|---------|---------|-------|--------------|-------------|
| **Braze Mobility** 🥇 | Blind Spot Sensors | $1,850–4,040 | Obstacle detection + multi-modal alerts | Rear-only, no WiFi, no app, no OTA |
| **Strutt EV1** | Semi-autonomous wheelchair | $8,000–15K | Full navigation, AI | Not retrofit, adult-focused |
| **Academic Research** | AI-IoT Wheelchair | N/A | Full ML + health monitoring | Research only, not commercial |
| **DIY Makers** | Custom Arduino/RPi | $50–200 | Varies | Untested, no app, poor docs |
| **Numotion** | Stock wheelchair | $2,000–5,000 | Mobility | No safety features |
| **CAREC (You)** 🚀 | Smart Safety Layer | **~$100** | **Everything** | Building it yourself (you can code!) |

### Why CAREC Wins

```
Braze ($4,000) vs CAREC (~$100):
├─ Same: Obstacle detection, beeping, mounting
├─ CAREC adds: WiFi OTA, ML camera, 1.45" display, BLE app
├─ CAREC adds: Home Assistant + Node-RED, on-premise LLM
├─ CAREC advantage: 40x cheaper, open-source, fully customizable
└─ Trade-off: You build it — but you're a 12-year embedded firmware expert!
```

---

## 📦 WHAT YOU'RE BUILDING

### The System

```
┌──────────────────────────────────────────────────────────┐
│           WHEELCHAIR SAFETY LAYER (CAREC)                │
└──────────────────────────────────────────────────────────┘
                           ↓
    ┌──────────────────────────────────────────────┐
    │   SenseCAP Watcher W1-A Clear Enclosure      │  $59.99
    │                                              │
    │  ┌─────────┐  ┌──────────┐  ┌────────────┐  │
    │  │OV5647   │  │Himax     │  │ESP32-S3    │  │
    │  │5MP 120° │→ │HX6538 NPU│→ │240 MHz     │  │
    │  │fixed 3m │  │on-device │  │8MB PSRAM   │  │
    │  └─────────┘  └──────────┘  └────────────┘  │
    │                                              │
    │  ┌────────────────────────┐  ┌───────────┐  │
    │  │ 1.45" Touch Screen     │  │ 1W Speaker│  │
    │  │ 412×412 px color alerts│  │ + Mic     │  │
    │  └────────────────────────┘  └───────────┘  │
    │                                              │
    │  WiFi 2.4GHz + Bluetooth 5  │  Grove IIC     │
    │  USB-C  │  micro-SD  │  GPIO expansion       │
    └──────────────────────────────────────────────┘
                           ↓
           Ball joint (1/4") + Tube clamp (1–1.5")
                           ↓
              Wheelchair Armrest (non-invasive)
                           ↓
              10000mAh LiPo battery via USB-C
                    (50+ hours runtime)
```

---

## 🛒 WHAT TO BUY (ALREADY DECIDED)

### Phase 1 Hardware (~$93–112)

```
✅ SenseCAP Watcher W1-A Clear Enclosure (SKU 113991315)
   Source: Seeed Studio (seeedstudio.com)
   Price:  $59.99
   Includes: Camera + Display + Speaker + Mic + ESP32-S3 + NPU + WiFi + BT + USB-C

🔄 LiPo Battery 10000mAh 3.7V
   Source: Amazon
   Price:  $15–25
   Purpose: 50+ hours runtime via USB-C

🔄 Ball Joint Mount (1/4" thread)
   Source: Amazon
   Price:  $10–15
   Purpose: 360° rotation + ±80° tilt adjustment

🔄 Tube Clamp (1–1.5" adjustable, stainless steel)
   Source: Amazon
   Price:  $8–12
   Purpose: Non-invasive clamp to wheelchair armrest

🔄 Micro-SD Card 32GB (optional)
   Source: Amazon
   Price:  $8–15
   Purpose: Local data logging (FAT32)
──────────────────────────────────────────────
TOTAL (excl. SD): ~$93–112
```

### Phase 2 Expansions (via Grove IIC — future)

```
⏳ Grove Accelerometer (ADXL345 or similar)
   Price: ~$5–10
   Purpose: Detect wheelchair motion → gate alerts (only beep when moving)

⏳ Grove HC-SR04 Ultrasonic (rear obstacle detection)
   Price: ~$5–8
   Purpose: 360° coverage extension

⏳ Grove IR Cliff Sensor
   Price: ~$5–8
   Purpose: Drop-off detection

Note: All connect to Watcher's Grove IIC port — no soldering required
```

---

## 📋 30-DAY ACTION CHECKLIST

### WEEK 1: HARDWARE SETUP

#### Days 1–2: Unbox + Connect
- [ ] Receive SenseCAP Watcher W1-A Clear Enclosure
- [ ] Power on: hold button 3 seconds → see 1.45" display
- [ ] Download SenseCraft Mate app (iOS/Android — free)
- [ ] Pair via Bluetooth in app
- [ ] Configure 2.4GHz WiFi in app
- [ ] Trigger OTA firmware update (app → Device Settings → Firmware)

#### Days 3–4: Validate Hardware
- [ ] Test built-in object detection (SenseCraft Mate → Live Camera)
- [ ] Point at furniture, person, wall — verify labels on display + app
- [ ] Test 1W speaker output (should be clearly audible at 2m)
- [ ] Clone Seeed repo: `git clone https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher.git`
- [ ] Set up Arduino IDE v2.x with Seeed board support (XIAO ESP32S3)
- [ ] Open `firmware/main/CAREC_main.cpp` — verify compiles

#### Days 5–7: Mount on Wheelchair
- [ ] Thread ball joint onto Watcher (1/4" standard mount)
- [ ] Wrap tube clamp around armrest — tighten (no drilling)
- [ ] Adjust camera: forward-facing, ~45° downward tilt
- [ ] Connect USB-C from 10000mAh LiPo battery
- [ ] Cable-manage USB-C along armrest (velcro ties)
- [ ] Confirm removable in under 2 minutes

---

### WEEK 2: CUSTOM FIRMWARE

#### Days 8–10: Obstacle Detection Logic
- [ ] Flash `firmware/main/CAREC_main.cpp` to Watcher via USB-C
- [ ] Verify Serial Monitor shows "CAREC ready." at 115200 baud
- [ ] Test `sensecraft_detect()` returns detections at 50cm, 80cm, 120cm
- [ ] Verify `nearest_obstacle_cm()` (bounding-box heuristic) returns correct distance range

#### Days 11–14: Beep + Display Alerts
- [ ] Verify `directional_beep_patterns.h`: BEEP_WARNING / BEEP_CRITICAL
- [ ] Verify `display_alert.h`: GREEN solid / YELLOW slow blink / RED fast blink
- [ ] Test all beep patterns via built-in 1W speaker
- [ ] Test at real distances: 50cm → RED+CRITICAL, 80cm → YELLOW+WARNING, 120cm → GREEN+silent
- [ ] Run `tests/obstacle_test.py` — all assertions pass

---

### WEEK 3: FIRMWARE CORE

#### Days 15–17: WiFi + BLE
- [ ] Verify WiFi reconnection logic works
- [ ] Test BLE GATT alert notifications to SenseCraft Mate app
- [ ] Confirm OTA check fires on WiFi reconnect
- [ ] Confirm OTA blocked during CRITICAL alert (safety-critical)

#### Days 18–21: Calibration
- [ ] Measure actual bounding-box pixel areas at 60cm and 100cm (zone boundaries)
- [ ] Update `distance_estimator.h` calibration constants
- [ ] Rerun `tests/obstacle_test.py` with updated constants
- [ ] Log 50 obstacle scenarios: measure accuracy (target >85%)

---

### WEEK 4: TESTING & VALIDATION

#### Days 22–25: Real-World Testing
- [ ] 50-obstacle scenario test matrix (furniture, walls, people)
- [ ] Measure false positive rate: 30-min empty-room session (target <5%)
- [ ] Measure latency: obstacle placed → first beep (target <200ms)
- [ ] Test speaker audible at 2m with wheelchair motor running
- [ ] Battery life test: 8+ hours continuous (10000mAh LiPo)

#### Days 26–28: Safety Validation
- [ ] Visual inspection: no loose wires, clear enclosure unobstructed
- [ ] Clamp stress test: shake armrest — Watcher must not shift
- [ ] Test BLE caregiver alerts: 100% receipt in 10 triggered scenarios
- [ ] OTA update test: deploy test firmware, verify install
- [ ] Caregiver sign-off

**END OF MONTH 1: Working CAREC MVP! 🎉**

---

### EXTENDED (Weeks 5–12): Full System

```
WEEK 5–6:
├─ Local LLM (Ollama + LLaMA-2) — on-premise scene description
├─ Sleep mode / power optimization (motion-gated detection)
└─ Phase 2: Add Grove accelerometer for motion gate

WEEK 7–8:
├─ Custom OTA pipeline (esp_https_ota + GitHub Actions)
├─ Caregiver BLE app extensions
└─ Home Assistant + Node-RED integration

WEEK 9–10:
├─ 72-hour burn-in test
├─ Failure mode analysis
└─ Safety protocol validation

WEEK 11–12:
├─ Documentation complete
├─ Caregiver training session
└─ Go/No-Go → Daily use
```

---

## 🎓 FIRMWARE STARTER (SenseCraft + CAREC)

```cpp
// CAREC_obstacle_detection.ino — SenseCAP Watcher W1-A
// ESP32-S3 + Himax HX6538 NPU + SenseCraft SDK

#include <Arduino.h>
#include <WiFi.h>
#include "sensecraft_detection.h"   // Seeed SenseCraft NPU interface
#include "distance_estimator.h"     // bounding-box → distance heuristic
#include "directional_beep_patterns.h"
#include "display_alert.h"          // RED/YELLOW/GREEN 1.45" screen driver
#include "motion_detector.h"        // optical-flow motion gate (fwd/bwd/stationary)
#include "ble_logger.h"             // BLE event logging to SenseCraft Mate
#include "wifi_ota.h"

#define DIST_RED    60   // cm — RED/YELLOW boundary
#define DIST_YELLOW 100  // cm — YELLOW/GREEN boundary
#define ZONE_GREEN  0
#define ZONE_YELLOW 1
#define ZONE_RED    2

const char* WIFI_SSID     = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    sensecraft_init();   // init Himax HX6538 NPU + OV5647 camera
    display_init();      // init 1.45" touch screen (starts GREEN)
    motion_init();       // init optical-flow motion gate
    ble_logger_init();   // init BLE GATT server for SenseCraft Mate
    Serial.println("CAREC ready.");
}

void loop() {
    // Motion gate — skip detection when wheelchair is stationary
    MotionState motion = motion_update();
    if (motion == MOTION_STATIONARY) {
        display_set_zone(ZONE_GREEN); display_update();
        beep_trigger(BEEP_NONE);      beep_update();
        ota_set_safe(true);           ota_check_and_update();
        delay(100);
        return;
    }

    DetectionResult result = sensecraft_detect();  // NPU on-device inference
    float dist = nearest_obstacle_cm(&result);     // bounding-box heuristic

    int zone = ZONE_GREEN;
    if      (dist < DIST_RED)    zone = ZONE_RED;
    else if (dist < DIST_YELLOW) zone = ZONE_YELLOW;

    beep_trigger(zone_to_beep(zone));
    beep_update();           // non-blocking beep state machine
    display_set_zone(zone);  // update display color
    display_update();        // non-blocking blink state machine
    ble_log_event(dist, zone, motion);  // send to SenseCraft Mate via BLE

    ota_set_safe(zone == ZONE_GREEN);
    ota_check_and_update();

    delay(100);  // 10 Hz safety loop
}
```

**Key advantage:** No separate sensors or wiring. Camera + NPU + speaker are all built into the SenseCAP Watcher. `sensecraft_init()` handles all hardware initialization.

---

## 📞 SUPPORT & RESOURCES

### SenseCAP Watcher Official
- **Product Page:** https://www.seeedstudio.com/SenseCAP-Watcher-W1-A-p-5979.html
- **GitHub (OSHW):** https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher
- **Wiki Setup Guide:** https://wiki.seeedstudio.com/sensecap_watcher/
- **Seeed Forum:** https://forum.seeedstudio.com/

### Development Tools
- **Arduino IDE v2.x** (recommended — quickest start)
- **ESP-IDF v5.x** (production/advanced)
- **SenseCraft Mate App** (iOS/Android — free, required for WiFi setup)
- **Ollama** (local LLM — free, privacy-first)
- **Home Assistant** (caregiver dashboard — Phase 2)

### Communities
- Seeed Studio Forum + GitHub Discussions
- r/esp32 (Reddit)
- Seeed Discord: https://discord.gg/seeed

---

## ⚠️ CRITICAL SAFETY NOTES

**Before first child use:**
- [ ] 1W speaker audible over wheelchair motor at 2m
- [ ] Clear enclosure unobstructed (no tape, no scratches on camera)
- [ ] Tube clamp tight — Watcher does not shift under vibration
- [ ] 10000mAh battery cannot be removed by child
- [ ] Caregiver can remove Watcher in under 2 minutes (emergency)
- [ ] 50+ obstacle scenarios tested before unsupervised use
- [ ] Caregiver knows all 3 beep patterns and display colors

---

*Last Updated: May 3, 2026 — SenseCAP Watcher W1-A Clear Enclosure edition*
