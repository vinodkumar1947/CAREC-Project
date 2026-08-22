// build_flags.h — CAREC compile-time feature flags
//
// Toggle subsystems here without editing source files.
// Safety-sensitive network features default off. Enable only in an explicitly
// reviewed development build; this file is not a production configuration.

#pragma once

// ── Core features ─────────────────────────────────────────────────────────────

// Motion sensing is not implemented on the current prototype.  Never use the
// always-forward stub to prove that the chair is stationary.
#define FEATURE_MOTION_GATE     0

// Enable BLE GATT event logging to SenseCraft Mate app.
// When 0, ble_log_event() is a no-op (saves ~40 KB flash).
#define FEATURE_BLE_LOGGING     1

// OTA remains disabled until maintenance-state gating, signed metadata,
// authenticated transport, rollback confirmation, and release evidence exist.
#define FEATURE_WIFI_OTA        0

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
