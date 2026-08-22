# Testing Rules — CAREC Firmware

## Build Gate
- `idf.py build` must pass with **zero errors** before any commit to main.
- Warnings in vendored components (Seeed_Arduino_SSCMA, ArduinoJson `containsKey`) are known and acceptable.
- Warnings in CAREC's own code (`firmware/main/`) are **not** acceptable — fix before committing.

## Python Tests
- Host-side tests live in `tests/` and use **pytest**.
- Run with: `python3 -m pytest tests/ -v`
- `tests/integration/obstacle_test.py` — validates distance threshold logic against zone boundaries.
- `tools/calibration/calibrate_distance.py` — run after any change to `distance_estimator.h` constants.

## Firmware Test Modes
Enable by defining ONE macro in `firmware/main/CAREC_main.cpp` (uncomment one line):

| Macro | Purpose |
|-------|---------|
| `DISPLAY_TEST` | SPD2010 LCD RED→YELLOW→GREEN cycle |
| `LED_TEST` | WS2813 25-color cycle on GPIO 40 |
| `SPEAKER_TEST` | ES8311 1500 Hz heartbeat beep |

These are mutually exclusive and take over `app_main()` entirely. Leave all three commented for production firmware.

## Hardware Bring-Up Sequence
When testing a new peripheral or after hardware changes:
1. Run `LED_TEST` first — confirms power rails and RMT are healthy.
2. Run `SPEAKER_TEST` next — confirms PCA9535 CODEC rail, I2C, I2S, ES8311.
3. Run `DISPLAY_TEST` — confirms PCA9535 LCD rail and QSPI SPI3.
4. Run normal firmware — confirms SSCMA SPI2, detection loop, BLE.

## Serial Monitor Checklist
After flashing, open monitor (`/project:monitor`) and verify:
- `[Detection] SSCMA OK` — Himax WE2 is alive
- `[Audio] ES8311 init OK` — codec ready
- `[LED] zone indicator on GPIO 40` — LED strip ready
- `[BLE] Advertising as "CAREC-Watcher"` — BLE stack up
- `[WiFi] Connected` or `Not connected — OTA disabled` — non-fatal either way
- `[Safety] X.X cm → GREEN/YELLOW/RED` appearing every ~100 ms — main loop running
