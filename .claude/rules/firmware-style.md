# Firmware Style Rules — CAREC / SenseCAP Watcher W1-A

## Toolchain
- **ESP-IDF v5.3.5** is the only supported build system. Never suggest Arduino IDE, PlatformIO, or arduino-cli.
- `#include <Arduino.h>` is valid — the project uses arduino-esp32 as a **managed component** inside IDF, not a separate IDE workflow.
- All build commands go through `idf.py`, not `arduino-cli` or `pio`.

## File Layout
- Firmware sources live in `firmware/main/*.{h,cpp}` — do not create `firmware/src/` or `firmware/lib/` (deleted 2026-05-10).
- Implementation headers (containing `static` functions) are acceptable in `firmware/main/` because each header is included by exactly one `.cpp` file.
- Config files belong in `firmware/config/` — secrets (e.g. `wifi_config.h`) are gitignored; only `*.template` is committed.

## C++ Conventions
- Use `#pragma once` — no `#ifndef` include guards.
- Prefer `esp_err_t` return values for driver-level functions; log failures with `ESP_LOGE(TAG, ...)` in pure IDF code.
- In `firmware/main/` (Arduino-on-IDF context), `Serial.printf` is acceptable for logging.
- Use `static` at file scope for module-private state and helpers.
- Use `inline` for small public API functions defined in headers.
- Use `[[noreturn]]` on infinite-loop functions (`led_test_run`, `speaker_test_run`).
- No `delay()` or blocking waits inside `carec_loop()` — use `vTaskDelay` only at the bottom of the loop.

## Module Architecture (ADR-001)
- All new firmware modules must follow the `tf_module_ops` vtable pattern — see `docs/specifications/ADR-001-tf-module-ops-architecture.md`.
- Do not add direct function calls between modules; use the Task Flow event bus (`tf_event_post`).
- Legacy direct-call modules (`sensecraft_detection.h`, etc.) are being migrated — do not add new direct-call modules.

## Comments
- Write no comments unless the WHY is non-obvious (hidden constraint, workaround, subtle invariant).
- Do not write what the code does — well-named identifiers do that.
- One-line max per comment block.
