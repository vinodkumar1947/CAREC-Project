# CAREC — System Architecture

**Version:** 0.3.0  
**Status:** Active development — Week 3  
**Last updated:** May 19, 2026

This document describes the application-level architecture of CAREC. It is written for developers and contributors who want to understand the system design, module responsibilities, and data flow. Hardware-specific details (chip models, GPIO numbers) are covered in [`docs/api/firmware_api.md`](api/firmware_api.md).

---

## System Overview

CAREC is a single-device safety add-on for electric wheelchairs. It runs a continuous detection loop that:

1. Captures video from a wide-angle forward-facing camera
2. Filters frames using a motion gate (skips stationary periods)
3. Runs on-device AI object detection (no cloud, no internet required)
4. Classifies detected objects into proximity zones
5. Delivers audio and visual alerts within 200ms
6. Logs zone events over Bluetooth to a companion mobile app

All processing is on-device. No video data leaves the device in any standard configuration.

---

## Four-Stage Detection Pipeline

```
┌─────────────────────────────────────────────────────────────────┐
│                     CAREC DETECTION LOOP                        │
│                   (runs every ~100ms while moving)              │
└─────────────────────────────────────────────────────────────────┘

  ┌─────────────┐     ┌──────────────────┐     ┌──────────────────┐     ┌─────────────────┐
  │  Stage 1    │     │    Stage 2        │     │    Stage 3        │     │    Stage 4       │
  │  Motion     │────▶│  On-Device AI     │────▶│  Distance         │────▶│  Alert          │
  │  Gate       │     │  Object Detection │     │  Classification   │     │  Delivery        │
  └─────────────┘     └──────────────────┘     └──────────────────┘     └─────────────────┘
       │                                                                         │
       │ Stationary?                                                      ┌──────┴──────┐
       ▼                                                                   │             │
    [SKIP —                                                           Audio alert  BLE log
   no alert,                                                         + display    to app
   allow OTA]                                                        update
```

### Stage 1 — Motion Gate

**Module:** `motion_gate.h`  
**Purpose:** Suppress the AI inference pipeline entirely when the wheelchair is not moving.

- Uses optical flow on a downsampled thumbnail (80×60 px, 12 feature points)
- If no significant motion is detected → set zone to CLEAR, silence audio, allow OTA check-in
- If forward or backward motion is detected → pass frame to Stage 2

**Why this matters:** Without the motion gate, a stationary wheelchair in a cluttered room would generate constant false positives. The gate eliminates this class of error entirely.

---

### Stage 2 — On-Device AI Object Detection

**Module:** `sensecraft_detection.h`  
**Purpose:** Identify objects in the camera frame using the on-device AI accelerator.

- Runs SSCMA (Seeed SenseCraft Model Assistant) object detection on the NPU
- Inference time: ~76ms measured on hardware
- Returns up to 8 detections with class label, confidence score, and bounding box
- Detections below `DETECTION_CONFIDENCE_THRESHOLD` (0.55) are discarded before Stage 3

**No cloud.** The model runs entirely on the local AI accelerator. No network connection is required or used for inference.

---

### Stage 3 — Distance Zone Classification

**Module:** `distance_estimator.h`  
**Purpose:** Convert AI detection results into a proximity zone.

- Uses a bounding-box area heuristic to estimate distance: larger bounding box = closer object
- Classifies into three zones:

| Zone | Estimated Distance | Trigger |
|------|-------------------|---------|
| CRITICAL | 0–60 cm | Largest acceptable bounding box threshold |
| WARNING | 60–100 cm | Medium bounding box threshold |
| CLEAR | > 100 cm | Small bounding box or no detection |

**Limitation:** Monocular camera heuristics are approximate. The estimator is calibrated against a physical test set (chairs, walls, people) during the Week 3–4 validation phase.

---

### Stage 4 — Alert Delivery

**Module:** `directional_beep_patterns.h` + `display_alert.h`  
**Purpose:** Deliver audio and visual feedback within 200ms of detection.

| Zone | Audio | Visual |
|------|-------|--------|
| CRITICAL | 3× fast beeps (repeating) | RED (fast blink) |
| WARNING | 1× slow beep (repeating) | YELLOW (slow blink) |
| CLEAR | Silence | GREEN (solid) |

After every alert decision, a Bluetooth event is logged to the companion mobile app (zone + timestamp, no images).

---

## Module Map

```
firmware/main/
├── CAREC_main.cpp              — Entry point, carec_setup() + carec_loop()
├── sensecraft_detection.h      — Stage 2: SSCMA inference client
├── motion_gate.h               — Stage 1: optical flow motion detection
├── distance_estimator.h        — Stage 3: bounding-box → zone classification
├── directional_beep_patterns.h — Stage 4: audio alert engine (ES8311 codec)
├── display_alert.h             — Stage 4: visual display (colour + blink pattern)
├── ble_logger.h                — Stage 4: Bluetooth event log to mobile app
└── ota_manager.h               — Background: WiFi OTA firmware update manager
```

Each module is header-only with a consistent interface:

```cpp
bool module_init();          // Called once in carec_setup()
void module_update(state);   // Called every loop iteration in carec_loop()
```

This makes each module independently testable and replaceable.

---

## Firmware Architecture Decision Record

**ADR-001** documents the accepted migration plan from the current direct-SDK module pattern to Seeed's `tf_module_ops` vtable pattern. This migration standardises the inter-module API and enables the use of Seeed's `tf_module_ai_camera` for SSCMA management.

See [`docs/specifications/ADR-001-tf-module-ops-architecture.md`](specifications/ADR-001-tf-module-ops-architecture.md) for full details.

---

## Connectivity Architecture

```
┌──────────────────────────────────────────────┐
│              CAREC DEVICE                    │
│                                              │
│  ┌──────────────┐   ┌──────────────────────┐ │
│  │  Detection   │   │    Wireless Layer    │ │
│  │  Pipeline    │   │                      │ │
│  │  (on-device) │   │  Bluetooth 5 ───────────▶ SenseCraft Mate app
│  │              │   │  (event log, config) │ │   (iOS / Android)
│  │  AI inference│   │                      │ │
│  │  <76ms       │   │  WiFi 2.4 GHz ──────────▶ OTA server
│  │              │   │  (OTA updates,       │ │   (automatic updates)
│  │  Alert < 200ms│  │   MQTT telemetry)    │ │
│  └──────────────┘   │                      │ │   MQTT ─────────────▶ Home Assistant
│                     │                      │ │                       Node-RED
│                     └──────────────────────┘ │
└──────────────────────────────────────────────┘
```

### Bluetooth (Bluetooth 5)
- **Purpose:** Real-time zone event logging to companion mobile app
- **What is sent:** Zone events (CRITICAL / WARNING / CLEAR) with timestamp — no images, no PII
- **Range:** ~30m open area
- **App:** SenseCraft Mate (free, iOS + Android)

### WiFi (2.4 GHz)
- **Purpose:** OTA firmware updates and optional MQTT telemetry
- **Required for core safety:** No — offline operation is fully functional
- **OTA:** Automatic check on device wake; applies updates with integrity verification

### MQTT (optional)
- **Purpose:** Telemetry publishing for Home Assistant and Node-RED dashboards
- **Topics:** Zone events, device status, battery level
- **Required:** No — disabled when no broker is configured

---

## Data Flow Summary

```
Camera frame
    │
    ▼
Motion Gate ──── stationary ──▶ CLEAR zone + silence + OTA allowed
    │
    │ moving
    ▼
AI Object Detection (on-device NPU, ~76ms)
    │
    ▼
Filter by confidence (threshold: 0.55)
    │
    ▼
Distance zone classification
    │
    ├──▶ CRITICAL → fast beeps + RED display + BLE log
    ├──▶ WARNING  → slow beep + YELLOW display + BLE log
    └──▶ CLEAR    → silence + GREEN display + BLE log
```

**End-to-end latency target:** < 200ms from obstacle entering detection zone to audio alert.

---

## Safety Design Principles

1. **Fail-safe default:** If any detection module fails to initialise, the system defaults to CRITICAL alert rather than silence.
2. **No blocking calls in the alert path:** The loop from detection to beep output contains no I/O waits, heap allocations, or network calls.
3. **Original controls preserved:** CAREC is read-only relative to the wheelchair — it never sends commands to the drive system.
4. **Caregiver override always available:** The companion app can silence alerts and adjust sensitivity at any time.
5. **Privacy by default:** No video data leaves the device; event logs contain zone events and timestamps only.

---

## Related Documentation

| Document | Purpose |
|----------|---------|
| [`docs/api/firmware_api.md`](api/firmware_api.md) | Public API for all 7 modules |
| [`docs/specifications/system_spec.md`](specifications/system_spec.md) | Functional and non-functional requirements |
| [`docs/specifications/ADR-001-tf-module-ops-architecture.md`](specifications/ADR-001-tf-module-ops-architecture.md) | Module architecture migration plan |
| [`docs/safety/safety_checklist.md`](safety/safety_checklist.md) | Pre-use and deployment safety checklist |
| [`docs/guides/programming_guide.md`](guides/programming_guide.md) | Build, flash, and development setup |
| [`firmware/README.md`](../firmware/README.md) | Firmware module structure and entry points |
