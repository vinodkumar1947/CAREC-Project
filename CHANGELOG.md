# Changelog

All notable changes to CAREC are documented here.

Format: `[vX.Y.Z] — YYYY-MM-DD` with sections Added / Changed / Fixed / Removed.

---

## [Unreleased] — Simulation-first program

### Changed

- Reset project direction around no-hardware contribution, a digital wheelchair, independent safety supervision, shared control, and supervised indoor navigation.
- Isolated physical adapters and releases under owner control.
- Classified the entries below as historical firmware-prototype records rather than the current autonomy roadmap.

> Versions 0.1.0–0.3.0 below describe the legacy SenseCAP warning-device prototype. Their production targets and schedules are not current.

## [0.3.0] — 2026-05-19 (Week 3 — Firmware Integration)

### Added
- `.claude/` project anatomy fully scaffolded: `settings.json`, `commands/`, `rules/`, `agents/`, `skills/`
- `/project:build`, `/project:flash`, `/project:monitor` slash commands with correct Python 3.11 venv and W1-A port
- Modular rule files: `firmware-style.md`, `hardware-constraints.md`, `safety-requirements.md`, `testing.md`
- Agent personas: `firmware-reviewer.md` (pin/blocking/SSCMA checks), `safety-auditor.md` (pre-deployment gate)
- `sensecap-watcher` project skill covering SSCMA client API, PCA9535 power sequencing, ES8311 codec, SPD2010 display, WS2813 RGB LED, and `tf_module_ops` migration
- `CLAUDE.local.md` (gitignored) — personal overrides for flash port and Python env path

### Changed
- Python venv migrated from `idf5.3_py3.9_env` → `idf5.3_py3.11_env` (Anaconda removed, Python 3.9 symlinks broken)
- `CLAUDE.md`, `firmware/SETUP.md`, `firmware/README.md` — all stale Python and status references updated
- `.gitignore` — `CLAUDE.local.md` and `settings.local.json` explicitly excluded

### Fixed
- Beep state machine bug in `directional_beep_patterns.h`: `!_beep_tone_on` guard fired on every silence phase, immediately restarting the tone. Fix: `beep_trigger()` now initialises `_beep_tone_on=true`; `beep_update()` rewritten as clean two-state machine (tone → silence → advance step → repeat)
- `display_alert.h` — copy-paste comment "ST7789" corrected to "SPD2010"

---

## [0.2.1] — 2026-05-18 (Week 3 — Build confirmation)

### Fixed
- Beep state machine silence phase not triggering — two-state machine rewrite (see 0.3.0 entry above)
- `display_alert.h` comment corrected

### Validated
- `idf.py build` passes with 0 errors, 0 warnings in CAREC sources
- `CAREC_firmware.bin` = 1.64 MB; 46% of 3 MB flash partition free
- 6× `containsKey()` deprecation warnings are inside vendored `Seeed_Arduino_SSCMA.cpp` (ArduinoJson API change — upstream issue)

---

## [0.2.0] — 2026-05-14 (Week 2 — ADR-001 + speaker validated)

### Added
- `docs/specifications/ADR-001-tf-module-ops-architecture.md` — architecture decision record for migrating all CAREC modules to Seeed's `tf_module_ops` vtable pattern
- `firmware/main/speaker_test.h` — PCA9535 power-on (SYSTEM + CODEC rails) + WS2813 + ES8311 init, 60-second heartbeat cycle
- `idf_component.yml` — added `espressif/led_strip ^3.0.0` for WS2813 RGB LED driver

### Changed
- `directional_beep_patterns.h` — major fix for ES8311 silent DAC: added missing analog power-up registers (`0x0E`, `0x12`, `0x0D`), speaker output route (`0x15=0x40`), corrected volume register (`0x32=0xBF`, unmuted), added `beep_ready()`, `beep_play_tone()`, `beep_play_silence()` helpers
- `display_alert.h` — backlight via LEDC PWM at 5 kHz / 10-bit / 80% duty (was direct GPIO HIGH; W1-A BL driver requires PWM); removed `SPICOMMON_BUSFLAG_QUAD`, bumped `max_transfer_sz` to full frame
- `CAREC_main.cpp` — `SPEAKER_TEST` flag added (three-way `#error` mutex with `DISPLAY_TEST` / `LED_TEST`)

### Known issues
- SPD2010 LCD panel remains dark after three iterations of fixes (PWM backlight, pre-driven pin LOW, mirror, bus flags). Hardware suspected (BL driver chip or ribbon cable). **Deferred.** RGB LED used as primary visual indicator.

---

## [0.1.1] — 2026-05-10 (Week 1 — Hardware arrived)

### Added
- `firmware/main/CAREC_main.cpp` — `carec_loop()` running with all 7 modules wired: SSCMA detection → distance estimate → zone → speaker beep + RGB LED + BLE log + OTA gate
- All 7 firmware modules compiled (stubs replaced with real SDK calls for detection, speaker, LED paths)
- SenseCraft Mate app paired; WiFi connected; OTA confirmed at latest firmware version

### Changed
- Distance thresholds calibrated to initial bench measurements (refinement ongoing in Week 3)
- BLE event logging active — zone events visible in SenseCraft Mate app

---

## [0.1.0] — 2026-04-28 (Hardware selection finalised)

### Added
- Full hardware selection documentation and rationale
- `docs/specifications/system_spec.md` — functional and non-functional requirements
- `docs/specifications/SENSECAP_WATCHER_HARDWARE_SELECTION.md` — competitor analysis (6+ options evaluated)
- `docs/guides/quick_reference.md` — project at a glance and 30-day checklist
- `docs/CAREC_tasks.csv` — 65 development tasks (ClickUp-ready)
- Initial firmware stubs for all 7 modules (compiles, no real SDK calls)

### Changed
- Architecture shifted from multi-sensor ESP32-C6 design to single all-in-one SenseCAP Watcher W1-A device

---

## Planned releases

| Version | Target | Focus |
|---------|--------|-------|
| `0.4.0` | Week 4 | 50-obstacle test matrix complete; distance thresholds calibrated; safety checklist signed off |
| `0.5.0` | Week 4 | 8-hour battery test passed; caregiver training materials complete |
| `1.0.0` | Q3 2026 | Production-ready; full documentation; research paper published |
