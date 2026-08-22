# Weekly Review — Week 02 (2026-05-11 to 2026-05-17)

## Metrics

| Metric | Target | Actual | Notes |
|--------|--------|--------|-------|
| Build hours logged | | | |
| Hardware tasks completed | | | |
| Firmware tasks completed | | 4 | LED, speaker, SSCMA, ES8311 driver fix |
| Obstacle scenarios tested | | | |
| Tests run | | | |
| Tests passed | | | |

---

## Build Phase Status

**Current Phase:**  [x] Phase 1 — Hardware Bring-up   [ ] Phase 2 — Full Loop Validation   [ ] Phase 3 — Field Testing

**Week target:** Wire beep + LED into main loop; validate end-to-end with real obstacles

---

## Wins (What worked well)

- WS2813 RGB LED on GPIO 40 — 25-colour cycle confirmed, `led_status.h` written
- ES8311 + 1W speaker — 1500 Hz heartbeat confirmed in `SPEAKER_TEST` mode
- SSCMA Himax HX6538 connected on first attempt via SPI2, 80ms inference
- PCA9535 power sequencing to Himax WE2 solid
- **Main loop flashed and running (2026-05-15)** — `CAREC ready.` confirmed, 12 Hz safety loop, BLE JSON events firing
- **SPD2010 LCD came up** — previously deferred as dark; `LCD panel create success v2.0.0` in this build
- BLE Nordic UART Service advertising as "CAREC-Watcher", JSON events verified in serial log
- All Seeed official docs linked into project (wiki + GitHub repos)
- `programming_guide.md` rewritten: pure ESP-IDF, no Arduino IDE references
- ADR-001 written: Seeed `tf_module_ops` architecture migration plan documented

## Blockers (What slowed me down)

- WiFi connection fails (assoc handshake timeout `0x400`/`0x2c0`) — OTA disabled, detection continues without it
- **Himax has Gesture Detection model loaded** (paper/rock/scissors, not person/obstacle) — SSCMA returns 0 boxes, dist_cm = 200 cm always. Need to swap model via SenseCraft Mate app before zone transitions will fire.

## Lessons Learned

- OV5647 feeds Himax over MIPI — ESP32-S3 never sees raw frames, so optical-flow motion gate is not implementable. Always-forward is correct for Phase 1.
- ES8311 requires both init and DAC-start register passes (matching esp-adf reference); partial init gives silent output
- idf_monitor requires a TTY; use Python pyserial to capture serial output from a non-interactive shell

---

## Hardware Progress

| Component | Status | Notes |
|-----------|--------|-------|
| SenseCAP Watcher W1-A | ✅ received + tested | May 4, 2026 |
| LiPo battery 10000mAh | ordered / received / tested | |
| Ball joint mount | ordered / received / tested | |
| Tube clamp | ordered / received / tested | |
| Micro-SD card | ordered / received / tested | |

---

## Firmware Progress

| Module | Status |
|--------|--------|
| SSCMA object detection (`sensecraft_detection.h`) | ✅ running — 80ms inference, Himax HX6538 via SPI2 |
| Distance threshold logic (`distance_estimator.h`) | ⚠️ pending — needs person/obstacle model on Himax first |
| Motion gate (`motion_detector.h`) | ✅ done — always-forward (optical flow N/A on Watcher) |
| WS2813 LED zone indicator (`led_status.h`) | ✅ running in main loop — GREEN confirmed (no obstacles) |
| Speaker beep patterns (`directional_beep_patterns.h`) | ✅ in main loop — BEEP_NONE confirmed; WARNING/CRITICAL pending model swap |
| Display zone colours (`display_alert.h`) | ✅ LCD live — `LCD panel create success v2.0.0` |
| BLE logging (`ble_logger.h`) | ✅ running — JSON events at ~220ms, "CAREC-Watcher" advertising |
| WiFi OTA (`wifi_ota.h`) | ⚠️ not connected — assoc handshake timeout |
| Main loop integration | ✅ flashed 2026-05-15, `CAREC ready.` confirmed, 12 Hz loop running |

---

## Safety Test Results

| Test | Pass/Fail | Distance Tested | Notes |
|------|-----------|-----------------|-------|
| LED GREEN solid (>100 cm) | ✅ PASS | clear room | Confirmed from serial log (no obstacles) |
| LED YELLOW blink (60–100 cm) | — | — | pending — need person/obstacle model on Himax |
| LED RED blink (<60 cm) | — | — | pending — need person/obstacle model on Himax |
| Speaker BEEP_WARNING | — | — | pending — need person/obstacle model on Himax |
| Speaker BEEP_CRITICAL | — | — | pending — need person/obstacle model on Himax |
| BLE JSON events firing | ✅ PASS | — | Verified in serial log at ~220ms cadence |
| False positive rate | — | — | target: <5% |
| Detection accuracy | — | — | target: >85% |
| Battery runtime | — | — | target: 50+ hrs |

---

## Next Week Plan

### Priority 1 — Swap Himax model (unblocks all zone tests)
- [ ] Open SenseCraft Mate app → device → AI Model
- [ ] Load "Person Detection" model (or obstacle model) onto Himax HX6538
- [ ] Confirm SSCMA returns non-zero boxes with person at 50/80/120 cm
- [ ] Confirm LED YELLOW blink at 60–100 cm, RED at <60 cm
- [ ] Confirm speaker BEEP_WARNING and BEEP_CRITICAL fire

### Priority 2 — Fix WiFi connection
- [ ] Check router for MAC filtering / try connecting phone to same AP to verify it's up
- [ ] Verify `wifi_config.h` SSID and password match current router config
- [ ] Confirm OTA check fires on WiFi connect

### Priority 3 — Field calibration
- [ ] Enable `DEBUG_BBOX_RAW 1`, run `tools/calibration/calibrate_distance.py`
- [ ] Update `CALIB_BBOX_AT_RED` and `CALIB_BBOX_AT_YELLOW` in `zone_config.h`

### Priority 4 — Wheelchair mounting
- [ ] Assemble ball joint + tube clamp
- [ ] Mount to wheelchair armrest (5 min)
- [ ] Adjust camera angle — forward, ~45° down, covering obstacle zone ahead of wheels

---

## Decisions Made This Week

| Decision | Rationale |
|----------|-----------|
| Adopt Seeed `tf_module_ops` architecture (ADR-001) | SenseCraft compatibility, reuse of `tf_module_ai_camera`, clean module boundaries |
| Defer optical-flow motion gate | OV5647 connects to Himax over MIPI — ESP32-S3 never sees raw frames; always-forward is correct for Phase 1 |
| SPD2010 LCD undeferred | Panel came up in the 2026-05-15 flash — `LCD panel create success v2.0.0` |
| Follow Seeed wiki + GitHub as primary reference | Avoids divergence from upstream BSP; all 9 official docs linked in project |

---

**Next Review:** 2026-05-17  
**90-Day Deadline Check:** Week 2 of 12 — on track
