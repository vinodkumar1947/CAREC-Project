# CAREC Project — Instructions for Claude

CAREC is a firmware project for the SenseCAP Watcher W1-A (ESP32-S3 + Himax HX6538). Read [`README.md`](README.md) for context and [`PROJECT_LOG.md`](PROJECT_LOG.md) for live status.

## Toolchain (don't get this wrong)

- **ESP-IDF v5.3.5** — committed in `firmware/sdkconfig.defaults`, used for all builds
- **Python 3.11 venv** at `~/.espressif/python_env/idf5.3_py3.11_env/` — recreated 2026-05-19 (old `idf5.3_py3.9_env` has broken Anaconda symlinks). Workaround: `export PATH="~/.espressif/python_env/idf5.3_py3.11_env/bin:$PATH"` then `source ~/esp/esp-idf/export.sh`. See [`CLAUDE.local.md`](CLAUDE.local.md) for the exact one-liner.
- **Flash port:** `/dev/cu.usbmodem5B142665543` (the higher-numbered port; lower-numbered one is the Himax WE2 console)
- **arduino-esp32 is a managed component**, not a separate Arduino IDE workflow. Treat `#include <Arduino.h>` in firmware as the IDF-on-arduino-component pattern.

## Project-installed skills

This repo ships skills under [`.claude/skills/`](.claude/skills/README.md). They are auto-available to Claude Code in this project. Prefer them over reinventing equivalent guidance:

| When working on… | Skill that should engage |
|---|---|
| SSCMA, PCA9535, ES8311, SPD2010, Himax WE2, tf_module_ops, SenseCraft detection | `sensecap-watcher` |
| W1-A pin assignments, GPIO/I²C/SPI/strap-pin questions, sdkconfig pin config | `gpio-config` |
| Firmware C++ in `firmware/main/*.{h,cpp}`, CMake, performance | `cpp-pro` |
| Host-side Python (`tools/calibration/`, `tools/log_parser/`, `tests/`) | `python-pro` |
| PR/code review, refactor audits | `code-reviewer` |
| OTA cert/signature handling, BLE pairing auth, secrets in `firmware/config/` | `secure-code-guardian` |
| System design, ADRs, big architectural changes | `architecture-designer` |
| Deep multi-source investigation (datasheets, vendor APIs) | `deep-research` |

If a skill's description matches the task, invoke it instead of working from generic priors. The `.claude/skills/README.md` index lists each skill's trigger conditions.

## Things to never do

- Don't suggest Arduino IDE / arduino-cli / PlatformIO workflows — the project is pure ESP-IDF
- Don't suggest GPIO 48 for any user feature — it's PCA9535 I²C SCL on the W1-A
- Don't suggest GPIO 47 for any user feature — it's PCA9535 I²C SDA on the W1-A
- Don't recreate `firmware/src/` or `firmware/lib/` — those were the Arduino-era layout, deleted on 2026-05-10. Canonical sources live in `firmware/main/`
- Don't commit `firmware/config/wifi_config.h` — gitignored; only `wifi_config.h.template` is tracked
- Don't auto-commit. Wait for explicit "commit" instruction.

## Hardware quick reference

The full pin map is in [`firmware/README.md`](firmware/README.md#hardware-reference). High-frequency entries:

| Peripheral | Pin(s) |
|---|---|
| RGB status LED (WS2813) | GPIO 40 |
| LCD QSPI (SPD2010) | CLK=7, D0=9, D1=1, D2=14, D3=13, CS=45, BL=8 |
| PCA9535 I²C0 | SDA=47, SCL=48, addr 0x21 |
| Himax SSCMA SPI2 | SCK=4, MISO=6, MOSI=5, CS=21 |
| Touch (CHSC6x) I²C | SDA=39, SCL=38 |

For standalone bring-up tests on the W1-A, see [`firmware/RGB_LED_BRINGUP.md`](firmware/RGB_LED_BRINGUP.md).

## Architecture decision

**ADR-001 (accepted May 14, 2026):** Migrate all CAREC firmware modules to Seeed's `tf_module_ops` vtable pattern (Task Flow engine). See [`docs/specifications/ADR-001-tf-module-ops-architecture.md`](docs/specifications/ADR-001-tf-module-ops-architecture.md) for the full plan, exact ABI structs, module mapping, and migration sequence. When writing any new firmware module, follow the `tf_module_*` pattern documented in ADR-001, not the old direct-call pattern.

## Seeed Official References

Consult these canonical sources before blog posts or Stack Overflow. If CAREC's approach diverges from Seeed's guidelines, flag it.

| Resource | URL | Use for |
|----------|-----|---------|
| OSHW Hardware | https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher | Schematics, BOM, factory flash recovery |
| Firmware BSP | https://github.com/Seeed-Studio/SenseCAP-Watcher-Firmware | Real driver APIs, PCA9535, SPD2010, pin defs |
| Product Overview | https://wiki.seeedstudio.com/watcher/ | Device capabilities, integration options |
| Software Framework | https://wiki.seeedstudio.com/watcher_software_framework/ | `tf_module_ops` vtable, `tf_event_post`, task-flow JSON — **primary architecture doc** |
| Module Dev Guide | https://wiki.seeedstudio.com/watcher_function_module_development_guide/ | Canonical recipe for adding new features |
| UI Integration | https://wiki.seeedstudio.com/watcher_ui_integration_guide/ | LVGL, `lv_pm_open_page`, `lvgl_port_lock`, view layer |
| SenseCraft App | https://wiki.seeedstudio.com/sensecap_app_introduction/ | BLE provisioning, HTTP alerts, cloud vs local AI |
| Service Framework | https://wiki.seeedstudio.com/watcher_software_service_framework/ | Data/Device Comm/Vision/Alert services, MQTT |
| Local Deploy | https://wiki.seeedstudio.com/watcher_local_deploy/ | Pointing Watcher at local LLM/VLM (not on-device flash) |
