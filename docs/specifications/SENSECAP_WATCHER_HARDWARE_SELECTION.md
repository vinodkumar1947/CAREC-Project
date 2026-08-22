# SenseCAP Watcher W1-A Hardware Selection
## CAREC Smart Wheelchair Safety System

**Status:** ✅ ORDERED & FINALIZED  
**Date:** April 28, 2026 (specs verified May 3, 2026)  
**Hardware:** SenseCAP Watcher W1-A Clear Enclosure — The Physical AI Agent for Smarter Spaces (SKU 113991315)  
**Decision Phase:** Complete

---

## Executive Summary

After comprehensive evaluation of AI camera modules (XIAO Vision AI Camera, reCamera, Luxonis OAK-D, Sipeed MaixCAM, etc.), the **SenseCAP Watcher W1-A** was selected as the primary hardware platform for CAREC wheelchair safety system.

**Why Watcher W1-A?**
- ✅ Dual-purpose device (experiments + wheelchair deployment)
- ✅ Built-in speaker + microphone (directional alerts)
- ✅ Open-source hardware + firmware (customizable)
- ✅ OTA firmware updates via WiFi
- ✅ LLM support (ChatGPT, local Ollama, DeepSeek)
- ✅ Clear enclosure (non-invasive mounting)
- ✅ Best price/performance ratio ($59.99)

---

## Hardware Specifications

### SenseCAP Watcher W1-A Clear Enclosure

```
PROCESSOR:
  - ESP32-S3 (dual-core Tensilica LX7 @ 240 MHz)
  - 8 MB PSRAM
  - 32 MB Flash (main)
  
AI ACCELERATOR:
  - Himax WiseEye2 HX6538
  - ARM Cortex-M55 + Ethos-U55 NPU
  - 16 MB Flash (AI firmware)

SENSORS / I/O:
  - 5MP OV5647 camera, 120° wide-angle FOV, fixed focus 3m
  - 1.45" touch screen, 412×412 px
  - Built-in microphone (3m pickup radius)
  - Built-in 1W speaker
  - Wheel button (navigation)

CONNECTIVITY:
  - WiFi 2.4 GHz (IEEE 802.11 b/g/n, ~100m range)
  - Bluetooth 5
  - USB-C (power + programming)
  - Micro-SD card slot (up to 32 GB FAT32)

POWER:
  - Input: 5V DC via USB-C
  - Onboard battery: Li-ion 3.7V, 400 mAh (~6 hrs standalone)
  - External LiPo: 3.7V recommended (10000mAh = 50+ hrs)
  - Avg. consumption: 150–200mA

EXPANSION:
  - 1× Grove IIC interface
  - 2×4 GPIO female header
  - UART access

ENCLOSURE:
  - Transparent (Clear) — W1-A variant
  - Compact: 69mm × 65mm × 20mm

ENVIRONMENT:
  - Operating temp: 0°C to 45°C
  - IP rating: Not rated (indoor use)

INTEGRATIONS:
  - SenseCraft (object detection, LLM, home automation)
  - XiaoZhi (voice AI companion)
  - Home Assistant
  - Node-RED
  - Arduino / ESP-IDF
  - On-premise deployment (private, no cloud required)
```

---

## Why NOT Other Cameras?

### Evaluated Alternatives (Rejected)

| Option | Price | Why Not |
|--------|-------|---------|
| **XIAO Vision AI Camera** | $24.90 | No speaker, no case, limited for wheelchair |
| **reCamera 2002w** | $54.90 | Overkill for obstacle detection, 480p only |
| **Raspberry Pi AI Camera** | $70 + Pi | RPi-locked, higher power consumption |
| **Luxonis OAK-D** | $150–300 | Overkill for wheelchair (3D depth not needed) |
| **Sipeed MaixCAM-Pro** | $80–100 | RISC-V dev tools, harder firmware porting |
| **Axis Communications** | $400–2000+ | Enterprise/surveillance grade, overkill |

---

## CAREC Hardware Evolution

### Phase 1: Original Design (DEPRECATED)
```
ESP32-C6 + HC-SR04 ultrasonic + Sharp GP2Y IR + MPU6050
→ Replaced: Too many sensors, unreliable in wheelchairs
```

### Phase 2: XIAO Investigation (REJECTED)
```
XIAO ESP32-S3 Sense + XIAO Vision AI Camera
→ Issue: No case, no speaker, limited ecosystem
```

### Phase 3: Final Selection (CURRENT)
```
SenseCAP Watcher W1-A
→ All-in-one solution with clear case, built-in I/O, open-source
```

---

## Complete Shopping List (ORDERED)

| Item | Qty | Price | Source | Status |
|------|-----|-------|--------|--------|
| **SenseCAP Watcher W1-A Clear Enclosure** (SKU 113991315) | 1 | $59.99 | Seeed Studio | ✅ ORDERED |
| **LiPo Battery (10000mAh, 3.7V)** | 1 | $15–25 | Amazon | 🔄 PENDING |
| **Ball Joint Mount (1/4")** | 1 | $10–15 | Amazon | 🔄 PENDING |
| **Tube Clamp (1–1.5" adjustable)** | 1 | $8–12 | Amazon | 🔄 PENDING |
| **USB-C Cable (if needed)** | 1 | $3–5 | Already have | ✅ SKIPPED |
| **Micro-SD Card (optional, 32GB)** | 1 | $8–15 | Amazon | 🔄 OPTIONAL |
| **—** | **—** | **—** | **—** | **—** |
| **TOTAL (excl. optional SD)** | **—** | **~$93–112** | **—** | **—** |
| **TOTAL (incl. optional SD)** | **—** | **~$103–127** | **—** | **—** |

### Battery Choice Rationale
- **10000mAh @ 150–200mA avg.** = 50+ hours continuous runtime
- Sufficient for multi-day wheelchair testing
- Compact form factor fits wheelchair saddlebag/pocket
- Standard USB-C charging

---

## Non-Invasive Wheelchair Mounting

### Mount Assembly

```
    ┌─────────────────────┐
    │  SenseCAP Watcher   │
    │  (clear case, 120°) │
    └──────────┬──────────┘
               │
        ╭──────┴────────╮
        │ Ball Joint     │
        │ (1/4" mount)   │
        │ ±80° tilt      │
        │ 360° rotate    │
        ╰────────┬───────╯
                 │
        ╭────────┴────────╮
        │ Tube Clamp      │
        │ 1–1.5" dia.     │
        │ Stainless steel │
        ╰────────┬────────╯
                 │
    ╭───────────┴──────────╮
    │  Wheelchair Armrest  │
    │  (PVC or metal)      │
    │  (no drilling)       │
    ╰──────────────────────╯
```

### Installation (5 minutes)
1. Loosen tube clamp bolts
2. Wrap clamp around armrest (1–1.5" diameter)
3. Tighten bolts evenly (finger-tight + 1/4 turn)
4. Screw ball joint to Watcher (1/4" mount)
5. Angle camera forward/down for optimal detection
6. **Fully removable anytime — zero damage to wheelchair**

### Mounting Advantages
- ✅ Non-invasive (no drilling, glue, or modifications)
- ✅ Removable in 2 minutes
- ✅ Adjustable angles (360° rotation, ±80° tilt)
- ✅ No impact on wheelchair operation
- ✅ Safe for 6-year-old user (no sharp edges)

---

## Firmware & Software Stack

### Official Firmware Options

| Firmware | Purpose | Best For |
|----------|---------|----------|
| **SenseCraft** | Vision AI + LLM integration | Custom CAREC code ✅ |
| **XiaoZhi** | Smart home companion | Chinese LLM ecosystem |

### CAREC Will Use: SenseCraft

**Why SenseCraft?**
- Better documented for custom code
- Supports English LLMs (ChatGPT, Ollama)
- Easier firmware porting from research prototypes
- Active Seeed community support

### LLM Integration Options (All FREE)

#### Option 1: OpenAI ChatGPT (Cloud)
```
Cost: ~$0.01–0.10 per image analysis
Pros: State-of-the-art accuracy
Cons: Requires internet, privacy concerns
Use case: R&D prototyping, not production wheelchair
```

#### Option 2: Local LLM (On-Device / PC)
```
Cost: FREE
Setup: Ollama + LLaMA-2 on local machine
Pros: Private, fast, no internet needed
Cons: Slower inference than cloud
Use case: CAREC wheelchair production ✅ RECOMMENDED
```

#### Option 3: Local TinyML Models
```
Cost: FREE
Models: YOLO, MobileNet on-device
Pros: Fastest, lowest power
Cons: Lower accuracy than LLMs
Use case: Real-time obstacle detection on Watcher
```

---

## OTA Firmware Updates

### ✅ Fully Supported

```
Update path: WiFi → SenseCraft app → Seeed servers
Updates include:
  - ESP32-S3 firmware
  - Himax HX6538 AI firmware
  - AI models
  - SenseCraft suite

Frequency: Monthly security patches + feature releases
Downgrade: NOT supported (backup before custom firmware)
Auto-check: When WiFi connected
```

### CAREC Update Strategy
1. **Pre-production:** Frequent updates + testing
2. **Production:** Lock to stable firmware + document version
3. **Backup:** Store original firmware on external device
4. **Testing:** Update test units first, then deploy to wheelchair

---

## Directional Beep Detection Logic (CAREC Firmware)

### Alert Patterns

```cpp
// Forward motion detection
if (direction == FORWARD) {
  beep_frequency = 500Hz;   // Higher pitch
  beep_pattern = FAST;       // Rapid beeps
  beep_interval = 200ms;
}

// Reverse motion detection
if (direction == REVERSE) {
  beep_frequency = 300Hz;   // Lower pitch
  beep_pattern = SLOW;       // Slower beeps
  beep_interval = 500ms;
}

// Distance-based escalation
if (distance < 60cm)   beep_rapid();     // RED zone — BEEP_CRITICAL
if (distance < 100cm)  beep_double();    // YELLOW zone — BEEP_WARNING
// 100+cm → ZONE_GREEN (silent)
```

### Direction Detection Implementation

| Method | Effort | Best For |
|--------|--------|----------|
| **Manual button/BLE toggle** | 1 hour | Initial prototype ✅ START HERE |
| **Accelerometer auto-detect** | 4 hours | Phase 2 upgrade |
| **Wheelchair speed signal** | TBD | Future integration |

---

## Development Setup

### Prerequisites
- Arduino IDE or ESP-IDF
- Seeed's XIAO board support (https://docs.seeedstudio.com/Seeed_Arduino_Board_Manager/)
- SenseCraft Mate app (free download, iOS/Android)
- USB-C cable for programming

### Getting Started
```bash
# 1. Download SenseCraft Mate app (FREE)
   iOS: https://apps.apple.com/us/app/sensecraft/id1619944834
   Android: https://play.google.com/store/apps/details?id=cc.seeed.sensecapmate

# 2. First boot setup
   - Connect Watcher via Bluetooth
   - Configure WiFi in app
   - Download latest firmware (OTA)

# 3. Test obstacle detection
   - Use SenseCraft's built-in object detection
   - Validate 120° FOV covers wheelchair front + sides
   - Confirm microphone picks up speech for alerts

# 4. Custom firmware (optional)
   - Clone Seeed's GitHub: https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher
   - Modify code for CAREC beep patterns
   - Compile with Arduino IDE / ESP-IDF
   - Upload via USB-C + SenseCraft suite
```

---

## Testing Roadmap

### Phase 1: Camera Validation (Week 1)
- [ ] Receive hardware + accessories
- [ ] Mount to wheelchair armrest
- [ ] Test 120° FOV (front, left, right)
- [ ] Confirm WiFi + BLE connectivity
- [ ] Load latest SenseCraft firmware

### Phase 2: Obstacle Detection (Week 2)
- [ ] Test object detection on obstacles
- [ ] Calibrate distance thresholds (60cm RED boundary, 100cm YELLOW boundary)
- [ ] Verify microphone + speaker audio
- [ ] Record obstacle detection accuracy (%)

### Phase 3: Custom Firmware (Week 3)
- [ ] Implement directional beep logic
- [ ] Test manual direction toggle (BLE)
- [ ] Integrate with local LLM (Ollama)
- [ ] Field test on actual wheelchair

### Phase 4: Production Integration (Week 4+)
- [ ] Stress testing (8–10 hours runtime)
- [ ] Wheelchair user acceptance testing
- [ ] Document safety procedures
- [ ] Deploy to Numotion wheelchair

---

## Competitor Analysis Summary

**Conclusion:** SenseCAP Watcher W1-A is the best option for CAREC because:

1. **Price-to-feature ratio** — Best value at $59.99
2. **Open-source ecosystem** — Fully customizable firmware
3. **Dual-purpose design** — Experiments + wheelchair deployment
4. **Built-in I/O** — Speaker + microphone without additional modules
5. **Community support** — Active Seeed Studio + GitHub presence
6. **OTA updates** — Secure WiFi firmware updates
7. **Clear case design** — Safe for non-invasive mounting

See `CAREC_competitive_analysis.md` for full comparison.

---

## GitHub Repository Structure

```
CAREC/
├── README.md                                  # Main project overview
├── HARDWARE.md                                # This file
├── docs/
│   ├── SENSECAP_WATCHER_HARDWARE_SELECTION.md
│   ├── WHEELCHAIR_SYSTEM_SPEC.md
│   ├── CAREC_COMPETITIVE_ANALYSIS.md
│   └── OTA_FIRMWARE_UPDATE_GUIDE.md
├── firmware/
│   ├── sensecraft_carec_obstacle_detection.ino
│   ├── directional_beep_patterns.h
│   └── local_llm_integration.cpp
├── hardware/
│   ├── mounting_assembly_guide.pdf
│   ├── ball_joint_specs.txt
│   └── tube_clamp_part_numbers.txt
├── tests/
│   ├── obstacle_detection_test.py
│   ├── audio_alert_validation.ino
│   └── wheelchair_integration_test.md
└── tasks/
    └── CAREC_tasks.csv                       # ClickUp-ready task list
```

---

## Quick Reference Links

### Official Documentation
- **Seeed SenseCAP Watcher:** https://www.seeedstudio.com/SenseCAP-Watcher-W1-A-p-5979.html
- **GitHub (Open-Source):** https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher
- **Wiki Setup Guide:** https://wiki.seeedstudio.com/sensecap_watcher/

### Software
- **SenseCraft Mate App:** https://play.google.com/store/apps/details?id=cc.seeed.sensecapmate
- **Arduino IDE Support:** https://docs.seeedstudio.com/Seeed_Arduino_Board_Manager/
- **Ollama (Local LLM):** https://ollama.ai/

### Community
- **Seeed Studio Forum:** https://forum.seeedstudio.com/
- **GitHub Discussions:** https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher/discussions

---

## Related Documentation

See also:
- `CAREC_COMPLETE_DOCUMENTATION.md` — Full system architecture
- `WHEELCHAIR_SYSTEM_SPEC.md` — Safety requirements + integration details
- `CAREC_competitive_analysis.md` — Detailed hardware comparison
- `CAREC_tasks.csv` — Development task breakdown (65 tasks)
- `SEEED_STUDIO_CAREC_SHOPPING.md` — Original shopping guide (pre-Watcher decision)

---

**Last Updated:** May 3, 2026  
**Decision Status:** FINAL ✅  
**Hardware Status:** ORDERED ✅
