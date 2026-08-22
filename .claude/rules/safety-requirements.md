# Safety Requirements — CAREC Wheelchair Safety System

CAREC is a **safety-critical system** used by a 6-year-old child. These rules are non-negotiable.

## Real-Time Response
- The full detection → alert cycle must complete in **≤ 100 ms** end-to-end.
- `carec_loop()` must never block. No `delay()`, no synchronous HTTP, no blocking I2C waits.
- Use `vTaskDelay(pdMS_TO_TICKS(100))` only at the **bottom** of the main loop, after all alert actions have been dispatched.
- SSCMA invoke is the only latency-critical operation; it's bounded by `perf.inference` (~80–150 ms on HX6538).

## Alert Path Integrity
- `beep_update()` **must** be called every loop iteration when a pattern is active — do not skip or gate it behind feature flags.
- `led_status_update()` **must** be called every loop iteration.
- BLE `ble_log_event()` must be non-blocking — it uses `notify()` which returns immediately whether or not a client is connected.
- Display `display_update()` should be called every loop but may be skipped if `_lcd_ready == false`.

## Zone Logic
- `ZONE_RED` (< 60 cm): BEEP_CRITICAL + fast blink. Never suppress.
- `ZONE_YELLOW` (60–100 cm): BEEP_WARNING + slow blink. Never suppress.
- `ZONE_GREEN` (> 100 cm): silence + solid green.
- Zone thresholds are in `firmware/config/zone_config.h` — do not hardcode distances in `carec_loop()`.

## OTA Updates
- OTA checks and downloads must **only** run when `zone == ZONE_GREEN` (clear path).
- `ota_set_safe(false)` must be called before starting any OTA flash operation.
- OTA failures must be silent and non-disruptive — set `_ota_safe_to_update = true` and continue detection.
- Do not add `delay()` or blocking waits in the OTA path.

## Sensor Failure Handling
- If SSCMA init fails, detection continues with `_ai_ready = false` — the device must stay functional (LED, beep, BLE still work).
- If ES8311 init fails, the audio path degrades gracefully — the device must still show visual alerts.
- Never halt or `abort()` due to a peripheral failure.

## Child Safety Checklist (pre-deployment)
Before testing with the child:
- [ ] Beep loud enough over wheelchair motor noise (test in real environment)
- [ ] No exposed wires or sharp components
- [ ] Battery cannot be pulled out accidentally
- [ ] IP54 enclosure integrity verified
- [ ] 50+ obstacle detection scenarios passed in supervised testing
- [ ] Parent briefed on all alert patterns
