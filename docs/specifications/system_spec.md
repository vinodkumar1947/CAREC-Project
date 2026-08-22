# CAREC — SMART WHEELCHAIR SAFETY SYSTEM: TECHNICAL SPECIFICATION
## AI-Powered Obstacle Detection for Electric Wheelchairs (Age 6+)

> **Legacy specification.** This April 2026 hardware plan is retained for provenance. Its schedules, costs, universal-compatibility wording, completion marks, production claims, and deployment plans are not current. Current scope is defined by the root README, ROADMAP, and `docs/vision/PROJECT_AIM.md`.

**Project Owner:** Vinod Kumar (IoT/Firmware Expert)  
**Target User:** 6-year-old child with Numotion electric wheelchair  
**Status:** Hardware Ordered → Development starts May 2026  
**Last Updated:** April 28, 2026

> **Hardware Decision (April 28, 2026):** After evaluating 6+ options, the build uses a
> **SenseCAP Watcher W1-A Clear Enclosure** ("The Physical AI Agent for Smarter Spaces") —
> all-in-one ESP32-S3 device with 5MP OV5647 camera, 1.45" touch screen, built-in 1W speaker,
> mic, WiFi, and Bluetooth 5. Replaces the original multi-sensor ESP32-C6 design.
> See [HARDWARE_STATUS_UPDATE.md](HARDWARE_STATUS_UPDATE.md) for full rationale.

---

## EXECUTIVE SUMMARY

A **non-invasive safety layer** that adds AI obstacle detection and caregiver monitoring to any electric wheelchair. The system mounts externally (no modification to the wheelchair) via a tube clamp on the armrest, provides OTA updates via WiFi, BLE alerts to the parent phone, and uses the SenseCraft on-device NPU for real-time inference.

**Key Innovation:** Single all-in-one device (SenseCAP Watcher W1-A) eliminates complex wiring. The Himax HX6538 NPU runs obstacle detection on-device at <150ms latency. No cloud required.

---

## SYSTEM REQUIREMENTS

### Functional Requirements
- ✅ Detect obstacles in wheelchair's path (forward-facing, 120° FOV)
- ✅ Alert child with beeping (3 patterns by danger level)
- ✅ Send danger alerts to parent's phone (BLE)
- ✅ On-device AI inference (Himax HX6538 NPU — no cloud dependency)
- ✅ OTA firmware updates via WiFi (SenseCraft Mate app)
- ✅ Non-invasive mounting (no drilling, removable in 2 minutes)
- ✅ Long battery runtime (50+ hours with external 10000mAh LiPo)

### Non-Functional Requirements
- ✅ Latency: <200ms (camera frame → audio alert)
- ✅ Detection accuracy: >85% (furniture, walls, people)
- ✅ False positive rate: <5%
- ✅ Battery runtime: 50+ hours (10000mAh LiPo @ 150–200mA draw)
- ✅ Recharge: USB-C
- ✅ BLE range: 30m (SenseCraft Mate app)
- ✅ WiFi: 2.4GHz home network (OTA, data logging)
- ✅ Enclosure: IP-rated clear plastic (built-in SenseCAP housing)

---

## SYSTEM ARCHITECTURE

```
┌─────────────────────────────────────────────────────────┐
│          CAREC: WHEELCHAIR SAFETY SYSTEM                │
│             (SenseCAP Watcher W1-A)                     │
├─────────────────────────────────────────────────────────┤
│
│  PERCEPTION LAYER
│  └─ 5MP OV5647 Camera (120° FOV, fixed focus 3m, built-in)
│     └─ Himax HX6538 NPU (Ethos-U55, on-device inference)
│        └─ SenseCraft object detection model
│
│  PROCESSING LAYER
│  └─ ESP32-S3 (dual-core 240 MHz, 8MB PSRAM, 32MB Flash)
│     ├─ Distance estimation (monocular, bounding-box heuristic)
│     ├─ Zone logic: RED (<60cm) / YELLOW (60–100cm) / GREEN (100+cm)
│     └─ Local LLM (Ollama, Phase 3)
│
│  FEEDBACK LAYER
│  ├─ Built-in 1W speaker — 2-tier beep patterns
│  ├─ 1.45" touch screen (412×412 px) — RED/YELLOW/GREEN zone display
│  └─ Built-in microphone — future: voice command
│
│  WIRELESS LAYER
│  ├─ WiFi 2.4GHz IEEE 802.11 b/g/n — OTA updates, data logging
│  └─ Bluetooth 5 — caregiver alerts (SenseCraft Mate app)
│
│  POWER LAYER
│  ├─ External LiPo 10000mAh 3.7V via USB-C
│  └─ Runtime: 50+ hours @ 150–200mA
│
│  MOUNTING
│  └─ Ball joint (1/4") + tube clamp (1–1.5") on armrest
│     Non-invasive, zero damage, removable in 2 minutes
│
└─────────────────────────────────────────────────────────┘
```

---

## SENSOR CONFIGURATION

### Primary Sensor: Built-in 5MP Camera

| Parameter | Value |
|-----------|-------|
| Camera | 5MP OV5647 |
| FOV | 120° horizontal |
| Focus | Fixed focus, 3m |
| Inference | Himax WiseEye2 HX6538 NPU (on-device) |
| Inference time | 80–150ms |
| Detection targets | Furniture, walls, people, obstacles |

### Built-in Display

| Component | Spec |
|-----------|------|
| Type | 1.45" touch screen |
| Resolution | 412×412 px |
| Use | Visual status, emoji alerts, live video feed |

### Built-in Audio I/O

| Component | Spec |
|-----------|------|
| Speaker | Built-in, 1W output |
| Microphone | Built-in, 3m pickup radius |

### Phase 2+ Expansion (Grove IIC + GPIO headers)

The SenseCAP Watcher exposes Grove IIC and GPIO headers for future expansion:

| Sensor | Purpose | Phase |
|--------|---------|-------|
| Grove ADXL345 accelerometer | Motion gate — suppress alerts when parked | Phase 2 |
| Ultrasonic HC-SR04 | Rear obstacle detection | Phase 2 |
| IR cliff sensor | Drop-off detection | Phase 2 |
| External buzzer | Louder alert if needed | Optional |

**Note — Grove Gesture Sensor not used:** The PAJ7620 Grove Gesture Sensor was evaluated and rejected. Its detection range is 5–15 cm, which requires waving a hand directly in front of the device — impractical on a moving wheelchair. Gesture recognition is handled instead by the built-in camera (see Gesture Detection below).

---

### Gesture Detection — Camera-Based (Phase 2)

**Decision:** Use the existing OV5647 camera + Himax HX6538 NPU for gesture recognition instead of a separate IR gesture sensor.

**Why camera beats Grove Gesture Sensor:**

| | Camera-based (OV5647) | Grove PAJ7620 |
|---|---|---|
| Detection range | 30–200 cm | 5–15 cm |
| Extra hardware | None | ~$8 module |
| Gesture vocabulary | Rich (pose estimation) | 9 directional swipes |
| Safety risk | Managed (stationary only) | None (dedicated HW) |

**Safety constraint:** The Himax HX6538 runs one inference model at a time. Switching from obstacle detection to gesture recognition while the wheelchair is moving would create a blind spot — a safety risk. Gesture detection is therefore only enabled when:

1. Wheelchair is confirmed **stationary** (Grove ADXL345 accelerometer, Phase 2)
2. Zone is **GREEN** (>100 cm — no obstacle in path)

When either condition breaks (motion detected or zone degrades), the system switches back to obstacle detection within one inference cycle (~100 ms).

**Intended gestures (Phase 2):**

| Gesture | Action |
|---------|--------|
| Open palm (hold) | Pause/resume alerts for 30 s |
| Wave (side to side) | Acknowledge current alert |
| Thumbs up | Confirm / dismiss |

**Implementation path:**

- **Phase 2 (near-term):** Time-multiplex — switch SSCMA model to Seeed hand-pose model when stationary + GREEN; switch back on motion. Requires accelerometer.
- **Phase 3 (long-term):** Single custom multi-task TFLite Micro model (obstacle classes + hand pose) running on Himax — no time-multiplexing, no safety gap.

**Build flag:** `FEATURE_GESTURE_DETECT` in `firmware/config/build_flags.h` (currently 0, enable in Phase 2).

---

## POWER MANAGEMENT

### Configuration

```
External LiPo 10000mAh 3.7V
    → USB-C → SenseCAP Watcher W1-A
    → Runtime: 50+ hours

SenseCAP internal battery:
    → ~6 hours (use only for testing, not daily use)
```

### Power Budget

| State | Current Draw | Runtime (10000mAh) |
|-------|-------------|-------------------|
| Active (camera + WiFi + BLE) | 150–200mA | 50–66 hours |
| Idle (BLE only) | ~50mA | 200+ hours |

**No sleep mode needed** — 50-hour runtime exceeds a full week of school days.

---

## FIRMWARE ARCHITECTURE

### Source Layout

```
firmware/main/
├── CAREC_main.cpp                 ← app_main() entry point (ESP-IDF)
├── sensecraft_detection.h         ← SSCMA person detection (Himax HX6538, SPI2)
├── distance_estimator.h           ← bounding-box → distance heuristic
├── display_alert.h                ← SPD2010 QSPI LCD zone colours (deferred — panel dark)
├── directional_beep_patterns.h    ← ES8311 I²S audio alerts (BEEP_WARNING / BEEP_CRITICAL)
├── motion_detector.h              ← Phase 1: always-forward (optical flow N/A on Watcher)
├── led_status.h                   ← WS2813 RGB zone indicator (primary visual feedback)
├── ble_logger.h                   ← BLE GATT Nordic UART Service JSON event stream
├── wifi_ota.h                     ← WiFi + HTTPS OTA (esp_https_ota)
├── rgb_led_test.h                 ← bring-up: WS2813 25-colour cycle (LED_TEST mode)
└── speaker_test.h                 ← bring-up: ES8311 heartbeat beep (SPEAKER_TEST mode)
```

**Toolchain:** ESP-IDF v5.3.5 (with `arduino-esp32` as a managed component). See `firmware/SETUP.md`.

### Safety Decision Logic (100ms loop)

```python
def safety_loop():
    # ── Step 1: Motion gate (optical flow — no accelerometer needed) ──────────
    motion = motion_update()   # sparse LK flow on 80×60 thumbnail, 12 features

    if motion == STATIONARY:
        display_set_zone(ZONE_GREEN)   # clear display
        beep(NONE)                     # silence
        ota_set_safe(True)             # allow OTA when stopped
        return                         # skip detection — wheelchair not moving

    # ── Step 2: Object detection (only when moving) ───────────────────────────
    detections  = sensecraft_detect(camera_frame)   # Himax HX6538 NPU
    distance_cm = nearest_obstacle_cm(detections)   # bounding-box heuristic

    # ── Step 3: Classify into alert zone ─────────────────────────────────────
    if distance_cm < 60:
        zone = ZONE_RED              # 0–60cm
    elif distance_cm < 100:
        zone = ZONE_YELLOW           # 60–100cm
    else:
        zone = ZONE_GREEN            # 100+cm

    # ── Step 4: Drive speaker ────────────────────────────────────────────────
    if zone == ZONE_RED:
        beep(CRITICAL)               # rapid burst, 2000Hz
        ble_log("DANGER: obstacle <60cm")
    elif zone == ZONE_YELLOW:
        beep(WARNING)                # 2 beeps/sec, 1500Hz
        ble_log("WARNING: obstacle 60-100cm")
    else:
        beep(NONE)

    # ── Step 5: Drive 1.45" touch screen ────────────────────────────────────
    display_set_zone(zone)           # RED blink / YELLOW blink / GREEN solid

    # ── Step 6: OTA + BLE logging ────────────────────────────────────────────
    ota_set_safe(zone == ZONE_GREEN)
    ble_log_event(distance_cm, zone, motion)

    # Phase 3: LLM scene description via speaker
    # if llm_enabled:
    #     speak(query_local_llm(camera_frame, "describe obstacle"))
```

### Alert Zones — Speaker + Display

| Zone | Distance | Speaker | Display | Blink |
|------|----------|---------|---------|-------|
| RED | 0–60cm | BEEP_CRITICAL: rapid burst, 2000Hz | RED fill | Fast 200ms on/off |
| YELLOW | 60–100cm | BEEP_WARNING: 2 beeps/sec, 1500Hz | YELLOW fill | Slow 500ms on/off |
| GREEN | 100+cm | Silent | GREEN fill | Solid on |

Speaker is built-in 1W — no external buzzer needed.  
Display is built-in 1.45" 412×412px — no external LED needed.

---

## CAREGIVER APP (SenseCraft Mate)

SenseCraft Mate (free, iOS/Android) provides:

- **Real-time BLE feed** — obstacle events as they happen
- **Alert history** — timestamped log of all detections
- **WiFi OTA** — trigger firmware update from phone
- **Device health** — battery, WiFi signal, firmware version

### Custom App (Future)

Phase 3+ goal: custom React Native app with:
- Indoor location tracking (BLE trilateration, ±1–5m)
- Parent notification push alerts
- Obstacle replay (what the camera saw)
- Sensitivity adjustment (distance thresholds)

---

## WiFi + OTA UPDATE SYSTEM

### SenseCraft OTA (Built-in)

```
1. Device connects to home WiFi (configured via SenseCraft Mate app)
2. SenseCraft checks for firmware updates automatically
3. OTA update downloads + verifies signature
4. Device reboots into new firmware (dual-partition, safe rollback)
5. Parent notified via app
```

### Custom OTA (Phase 3)

For fully custom CAREC firmware, ESP-IDF `esp_https_ota`:
- HTTPS endpoint with ECDSA-signed firmware binaries
- Automatic weekly check
- Rollback on failed boot

---

## TECHNICAL SPECIFICATIONS

### Main Controller: ESP32-S3 (inside SenseCAP Watcher W1-A Clear Enclosure)

```
Processor:      dual-core Xtensa LX7, 240 MHz
Memory:         8 MB PSRAM, 32 MB Flash
WiFi:           2.4GHz IEEE 802.11 b/g/n
Bluetooth:      Bluetooth 5 (30m range)
USB:            USB-C (programming + power)
Storage:        Micro-SD slot, up to 32 GB FAT32
Expansion:      1× Grove IIC + 2×4 GPIO female header
Operating temp: 0°C to 45°C
```

### AI Accelerator: Himax WiseEye2 HX6538

```
NPU:            ARM Ethos-U55
Core:           ARM Cortex-M55
Flash:          16 MB (AI firmware)
Models:         TFLite Micro, SenseCraft detection models
Inference time: 80–150ms per frame
```

### Camera: OV5647

```
Resolution:     5MP
FOV:            120° horizontal
Focus:          Fixed focus, 3m
Interface:      MIPI CSI (internal to device)
```

### Display

```
Size:           1.45" touch screen
Resolution:     412×412 px
Input:          Touch + wheel button
```

### Audio

```
Speaker:        built-in, 1W output
Microphone:     built-in, omni, 3m pickup radius
```

### Full Device: SenseCAP Watcher W1-A Clear Enclosure

```
Product name:   SenseCAP Watcher W1-A Clear Enclosure
                "The Physical AI Agent for Smarter Spaces"
SKU:            113991315
Enclosure:      Transparent (Clear), 69×65×20mm
Onboard battery: Li-ion 3.7V, 400 mAh
Power input:    5V DC via USB-C, 150–200mA avg
```

---

## BILL OF MATERIALS

See [hardware/BOM.md](../../hardware/BOM.md) for the current parts list.

**Summary:** SenseCAP Watcher W1-A Clear Enclosure + ball joint + tube clamp + 10000mAh LiPo = **~$93–112 total** (+ $8–15 optional micro-SD).

No breadboard, wiring, sensors, or soldering required — the SenseCAP Watcher integrates all components.

---

## MARKET COMPARISON

| Feature | CAREC | Braze Mobility | Academic Research | DIY (ESP32 custom) |
|---------|-------|----------------|-------------------|--------------------|
| **Price** | ~$93–127 | $1,500–3,000 | N/A | $100–150 |
| **AI vision** | ✅ 5MP + NPU | ❌ No camera | ✅ Varies | ⚠️ Basic camera |
| **OTA Updates** | ✅ WiFi OTA | ❌ None | ⚠️ Manual | ⚠️ Manual |
| **All-in-one** | ✅ Yes | ❌ Add-on kit | ⚠️ Prototype | ❌ Custom wiring |
| **Setup time** | 5 min | 1–2 hrs | Days | Weeks |
| **Open source** | ✅ Yes | ❌ Closed | ✅ Research | ✅ Yes |
| **Caregiver app** | ✅ SenseCraft Mate | ⚠️ Basic | ⚠️ Varies | ❌ DIY |
| **Kid-focused** | ✅ Yes | ⚠️ Adult-focused | ❌ No | ❌ No |

---

## 90-DAY BUILD ROADMAP

### Phase 1: Hardware Setup (Week 1–2, May 2026) ← **CURRENT**

**Goal:** Validate SenseCAP Watcher W1-A, mount on wheelchair

```
Week 1: Hardware Arrives
├─ Unbox SenseCAP Watcher W1-A
├─ Download SenseCraft Mate app
├─ Pair via Bluetooth, configure WiFi
├─ Verify camera 120° FOV + built-in object detection
├─ Test speaker output (1W built-in)
└─ OTA update to latest SenseCraft firmware

Week 2: Wheelchair Mounting
├─ Assemble ball joint + tube clamp
├─ Mount to Numotion armrest (5 minutes)
├─ Adjust camera angle (forward, ~45° down)
├─ Test FOV covers obstacle zone ahead of wheels
└─ Verify speaker audible over wheelchair motor
```

**Deliverable:** Device mounted, basic obstacle detection working via SenseCraft app  
**Cost:** Already ordered — $144–192

---

### Phase 2: Custom Firmware (Weeks 3–4, May 2026)

**Goal:** CAREC zone detection logic, RED/YELLOW/GREEN display + 2-tier beep alerts

```
Week 3: Obstacle Detection
├─ Implement zone thresholds in CAREC_obstacle_detection.ino (60cm / 100cm)
├─ Implement display_alert.h: RED fast blink / YELLOW slow blink / GREEN solid
├─ Implement directional_beep_patterns.h (BEEP_CRITICAL + BEEP_WARNING)
├─ Run tests/obstacle_test.py — all pass
└─ Test with obstacles at 30cm (RED), 80cm (YELLOW), 120cm (GREEN)

Week 4: Field Testing
├─ 50-obstacle scenario test (furniture, walls, people)
├─ Measure detection accuracy (target: >85%)
├─ Measure false positive rate (target: <5%)
├─ 8-hour continuous runtime test
└─ Supervised test with child
```

**Deliverable:** Working CAREC firmware with beep alerts  
**Cost:** $0 (software only)

---

### Phase 3: LLM + OTA + App (Weeks 5–8, June 2026)

**Goal:** Local LLM scene description, custom OTA, parent app

```
Week 5–6: Local LLM
├─ Implement local_llm_integration.cpp
├─ Test ChatGPT API (dev: ~$0.01–0.10/image)
├─ Set up Ollama + LLaMA-2 locally (production, free)
└─ Measure LLM response latency (target: <2s)

Week 7–8: OTA + Custom App
├─ Custom OTA update pipeline (ESP-IDF esp_https_ota)
├─ Build parent notification prototype
└─ Indoor location tracking (BLE trilateration)
```

**Deliverable:** Full featured system  
**Cost:** ~$0 (software + free Ollama)

---

### Phase 4: Hardening (Weeks 9–12, July 2026)

**Goal:** Production-ready, safety-validated, daily use

```
Week 9–10: Safety Validation
├─ Formal test protocol (all scenarios below)
├─ 72-hour continuous run
├─ Edge case testing (low light, cluttered room)
└─ Failure mode testing

Week 11: Documentation
├─ Complete firmware docs
├─ Parent user manual
└─ Safety procedures

Week 12: Launch
├─ Final integration with Numotion wheelchair
├─ Parent training
└─ Go-live for daily use
```

**Deliverable:** Production-ready system for daily use by child

---

## TESTING PROTOCOL

### Pre-Child-Use Tests

| Test | Method | Target |
|------|--------|--------|
| Detection accuracy | 50 obstacle scenarios | >85% |
| False positive rate | 30-min session, no obstacles | <5% |
| Latency | Obstacle placed, time to first beep | <200ms |
| Battery runtime | Continuous operation from full charge | 50+ hours |
| BLE alert delivery | Trigger 50 alerts, count received on phone | 100% |
| Speaker volume | Measure dB at 2m with motor running | Audible |
| OTA update | Deploy new firmware version, verify install | Success |
| Mount stability | Shake test, bump over threshold | No wobble |
| Low light | Test detection in dim room (night light only) | >70% accuracy |
| Wet conditions | Splash test (rain simulation) | No failure |

---

## SUCCESS CRITERIA

System is ready for daily use when:

- ✅ >85% obstacle detection accuracy (tested over 50 scenarios)
- ✅ <5% false positive rate
- ✅ <200ms latency (camera frame → audio alert)
- ✅ 50+ hour battery runtime verified
- ✅ Speaker audible at 2m with wheelchair motor running
- ✅ BLE alerts reliably received by parent
- ✅ WiFi OTA updates working end-to-end
- ✅ Mount secure under normal wheelchair operation
- ✅ Child understands 2 beep patterns and 3 display colors (RED/YELLOW/GREEN)
- ✅ No safety incidents in supervised testing
- ✅ Parent confident to use independently

---

**END OF SPECIFICATION**

*This is a living document — updated as hardware decisions and test results come in.*
