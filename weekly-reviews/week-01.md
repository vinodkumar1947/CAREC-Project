# Weekly Review — Week 1 (2026-05-04 to 2026-05-10)

**Hardware arrived: May 4, 2026** 🎉  
**Week target:** Hardware validated + dev environment ready + mounted on wheelchair

---

## Metrics

| Metric | Target | Actual | Notes |
|--------|--------|--------|-------|
| Build hours logged | 10 | | |
| Hardware tasks completed | 6 | | Unbox / Pair / OTA / Camera / Speaker / Display |
| Firmware tasks completed | 1 | | Compile + dry-upload |
| Obstacle scenarios tested | 3 | | 30cm / 80cm / 120cm via SenseCraft Mate app |
| Seeed API calls identified | 3 | | sensecraft_init / sensecraft_detect / display API |

---

## Build Phase Status

**Current Phase:** ✅ Phase 1 — Hardware Setup

**Week target from 90-day roadmap:**  
Receive → Pair → OTA → Validate I/O → Mount → Dev Environment ready

---

## Day-by-Day Checklist

### Day 1 — TODAY (May 4) : Unbox + Connect

- [x] Unbox — verify contents: Watcher W1-A + USB-C cable
- [x] First power-on: hold button 3 seconds → 1.45" display lights up
- [x] Verify startup sound from 1W speaker
- [x] Inspect OV5647 camera lens (no dust/scratches on clear enclosure)
- [x] Download SenseCraft Mate app (iOS/Android — free)
- [x] Pair via Bluetooth in app → Add Device → SenseCAP Watcher
- [x] Configure 2.4 GHz WiFi (⚠️ 5 GHz NOT supported)
- [x] **Record firmware version shown in app:** `__AI FW___2024.08.16__`
- [x] esp fw 1.1.7

### Day 2 (May 5) : OTA Update + Built-in Validation

- [x] SenseCraft Mate → Device Settings → Firmware → Check for Updates
- [x] Install latest SenseCraft firmware — verify clean reboot
- [x] **Record updated firmware version:** `___1.1.7_______`
- [x] Record updated Himax HX6538 NPU model version: `____2024.08.16______`
- [x] SenseCraft Mate → Live Camera — verify 120° FOV visible
- [x] Enable SenseCraft built-in object detection model
- [ ] Point at: chair ✓  person ✓  wall ✓  door frame ✓ — labels appear on display + app
- [ ] Test at 30 cm (RED zone) — labels visible + inference < 150ms
- [ ] Test at 80 cm (YELLOW zone) — still detecting
- [ ] Test at 120 cm (GREEN zone) — still detecting

### Day 3 (May 6) : Dev Environment + Seeed Reference Firmware

- [ ] Clone Seeed OSHW repo:
  ```bash
  git clone https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher.git
  cd OSHW-SenseCAP-Watcher
  ```
- [ ] Locate and read: ESP32-S3 firmware entry point (`main.cpp` or `app_main`)
- [ ] **Find the real `sensecraft_detect()` API call** — note function name here: `_______________`
- [ ] **Find the real display API** — note how to draw color fills: `_______________`
- [ ] **Find the real speaker/PWM GPIO** — note pin number: `_______________`
- [ ] Find the SenseCraft detection result struct — confirm field names match `sensecraft_detection.h`
- [ ] Install ESP-IDF v5.3.5 (`./install.sh esp32s3`)
- [ ] `. ~/esp/esp-idf/export.sh` to source the env
- [ ] `idf.py set-target esp32s3` (one-time per checkout)

### Day 4 (May 7) : Compile + First Flash

- [ ] Copy WiFi config:
  ```bash
  cp firmware/config/wifi_config.h.template firmware/config/wifi_config.h
  # Edit with your SSID + password
  ```
- [ ] `cd firmware && idf.py build` — **must compile with zero real errors**
- [ ] Connect Watcher via USB-C data cable
- [ ] `idf.py -p /dev/cu.usbmodem<NNNN> flash` → verify upload succeeds
- [ ] Serial Monitor at 115200 baud — verify:
  ```
  CAREC starting...
  [WiFi] Connected: ...
  [BLE] Init OK ...
  CAREC ready.
  ```
- [ ] **Note: stubs are active** — detections return 0, display/beep do nothing yet. That is expected.

### Days 5–7 (May 8–10) : Mount + Speaker Confirm + API Stub Replacement Starts

- [ ] Measure wheelchair armrest diameter: `_____ inches`
- [ ] Thread ball joint onto Watcher (1/4" standard mount)
- [ ] Wrap tube clamp around armrest — tighten
- [ ] Adjust camera: forward-facing, ~45° downward tilt
- [ ] Confirm 120° FOV covers obstacle zone ahead of wheels (check Live Camera)
- [ ] Connect USB-C from 10000mAh LiPo battery
- [ ] Secure cable along armrest with velcro ties — away from wheels
- [ ] Shake test: Watcher must not shift
- [ ] **Confirm speaker GPIO pin** from Seeed OSHW repo — update `#define SPEAKER_PIN` in sketch
- [ ] Begin replacing `sensecraft_init()` stub with real SDK call (see Day 3 notes)

---

## Wins (fill in as you go)

- Hardware received on schedule ✓
- 
- 

## Blockers

- 
- 

## Critical Unknowns to Resolve This Week

| Unknown | Where to find it | Status |
|---------|-----------------|--------|
| Real `sensecraft_detect()` API call | Seeed OSHW repo → firmware/main/ | ⬜ |
| Display draw API (color fill) | Seeed OSHW repo → display examples | ⬜ |
| Speaker GPIO pin number | Seeed OSHW repo → schematic / pinout | ⬜ |
| SenseCraft detection result struct fields | Seeed OSHW repo headers | ⬜ |
| BLE GATT UUIDs for SenseCraft Mate | Seeed forum / contact Seeed support | ⬜ |

---

## Hardware Progress

| Component | Status | Notes |
|-----------|--------|-------|
| SenseCAP Watcher W1-A | **received** | May 4, 2026 |
| LiPo battery 10000mAh | ordered / received / tested | |
| Ball joint mount | ordered / received / tested | |
| Tube clamp | ordered / received / tested | |
| Micro-SD card | ordered / received / tested | |

---

## Firmware Progress

| Module | Status | Blocker |
|--------|--------|---------|
| `sensecraft_detection.h` — init | stub | Need Seeed SDK API |
| `sensecraft_detection.h` — detect | stub | Need Seeed SDK API |
| `display_alert.h` | stub | Need Seeed display driver API |
| `directional_beep_patterns.h` | stub | Need GPIO pin confirmed |
| `motion_detector.h` | stub | Need `esp_camera_fb_get()` tested |
| `ble_logger.h` | stub (Serial mirror) | Need UUID from Seeed |
| `wifi_ota.h` | stub | WiFi connects, OTA not wired |
| `distance_estimator.h` | code complete | Needs calibration constants |

---

## Safety Test Results

| Test | Pass/Fail | Notes |
|------|-----------|-------|
| Obstacle at 30cm → RED zone | | Via SenseCraft Mate app (before custom firmware) |
| Obstacle at 80cm → YELLOW zone | | |
| Obstacle at 120cm → GREEN zone | | |
| 1W speaker audible at 2m | | |
| Camera 120° FOV adequate | | |
| Clamp stays under vibration | | |

---

## Decisions Made This Week

| Decision | Rationale |
|----------|-----------|
| | |

---

## Next Week Plan (Week 2)

### Priority 1 — Replace all stubs with real API calls
- [ ] `sensecraft_detection.h` → real `sensecraft_detect()` call
- [ ] `display_alert.h` → real display color/blink driver
- [ ] `directional_beep_patterns.h` → real speaker GPIO (ledc/PWM)
- [ ] `motion_detector.h` → real `esp_camera_fb_get()`

### Priority 2 — Verify end-to-end loop works on hardware
- [ ] Serial shows real detection labels (not empty)
- [ ] Display changes colour when obstacle placed
- [ ] Speaker beeps at correct pattern

### Priority 3 — Run initial calibration
- [ ] Enable `DEBUG_BBOX_RAW 1`
- [ ] Run `tools/calibration/calibrate_distance.py`
- [ ] Update `CALIB_BBOX_AT_RED` + `CALIB_BBOX_AT_YELLOW` in `zone_config.h`

---

**Next Review:** 2026-05-11  
**90-Day Deadline Check:** Week 1 of 12
