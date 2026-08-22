// build_flags.h — CAREC compile-time feature flags
//
// Toggle subsystems here without editing source files.
// All flags default to enabled (1). Set to 0 to disable.

#pragma once

// ── Core features ─────────────────────────────────────────────────────────────

// Enable the optical-flow motion gate.
// When 0, detection runs every loop (wastes power, higher false-positive rate).
#define FEATURE_MOTION_GATE     1

// Enable BLE GATT event logging to SenseCraft Mate app.
// When 0, ble_log_event() is a no-op (saves ~40 KB flash).
#define FEATURE_BLE_LOGGING     1

// Enable WiFi + OTA firmware updates.
// When 0, WiFi is not initialised (lower boot time, no OTA).
#define FEATURE_WIFI_OTA        1

// Enable the 1.45" display driver.
// When 0, display_update() is a no-op (useful for headless bench testing).
#define FEATURE_DISPLAY         1

// ── Debug / development flags ─────────────────────────────────────────────────

// Print verbose zone + distance logs to Serial every loop.
// Disable for production to reduce Serial flood.
#define DEBUG_VERBOSE           1

// Print raw bounding-box coordinates to Serial.
// Only useful when calibrating distance_estimator.h constants.
#define DEBUG_BBOX_RAW          0

// ── Phase 2 / future features ─────────────────────────────────────────────────

// Grove accelerometer motion gate (replaces optical-flow gate when connected).
// Enable after wiring Grove ADXL345 to IIC port.
#define FEATURE_ACCEL_GATE      0

// Camera-based gesture detection via Himax SSCMA hand-pose model.
// Only activates when FEATURE_ACCEL_GATE=1 AND wheelchair is stationary AND zone=GREEN.
// Switches SSCMA model from obstacle detection to hand-pose; switches back on motion.
// See docs/specifications/system_spec.md — Gesture Detection section.
#define FEATURE_GESTURE_DETECT  0

// Local LLM scene description via Ollama (Phase 2 — Week 5-6).
#define FEATURE_LOCAL_LLM       0

// Micro-SD data logging (Phase 2 — requires 32 GB micro-SD in slot).
#define FEATURE_SD_LOGGING      0

// Grove ultrasonic rear sensor (Phase 2 — 360° coverage).
#define FEATURE_REAR_ULTRASONIC 0
