// zone_config.h — CAREC alert zone thresholds
//
// Centralises all distance and zone constants.
// Include this in the main sketch and any module that needs zone definitions.
//
// Zone layout:
//   ZONE_RED    : 0 – DIST_RED cm       → RED display (fast blink) + BEEP_CRITICAL
//   ZONE_YELLOW : DIST_RED – DIST_YELLOW → YELLOW display (slow blink) + BEEP_WARNING
//   ZONE_GREEN  : DIST_YELLOW+ cm        → GREEN display (solid on) + silence
//
// Classification test points (not physical accuracy claims):
//   30 cm  → RED zone
//   80 cm  → YELLOW zone
//  120 cm  → GREEN zone
//
// After changing values, run tests/obstacle_test.py. Physical calibration and
// validation require a separate owner-controlled protocol.

#pragma once

// ── Distance boundaries (cm) ─────────────────────────────────────────────────

#define DIST_RED     60     // 0–60 cm  → RED zone
#define DIST_YELLOW 100     // 60–100 cm → YELLOW zone; 100+ cm → GREEN zone

// ── Zone identifiers ─────────────────────────────────────────────────────────

#define ZONE_GREEN   0
#define ZONE_YELLOW  1
#define ZONE_RED     2

// ── Bounding-box calibration constants (for distance_estimator.h) ────────────
// Derived from camera calibration with a 30 cm wide reference object (chair leg).
// Update these after running tools/calibration/calibrate_distance.py.

// bbox_w_norm at DIST_RED   boundary (60 cm): measured 0.241
#define CALIB_BBOX_AT_RED     0.241f

// bbox_w_norm at DIST_YELLOW boundary (100 cm): measured 0.144
#define CALIB_BBOX_AT_YELLOW  0.144f

// Camera half-FOV: OV5647 120° → 60° half-angle
#define CALIB_HALF_FOV_DEG   60.0f

// Physical width of the reference calibration object (cm)
#define CALIB_REF_WIDTH_CM   30.0f

// ── Beep pattern loop parameters ─────────────────────────────────────────────

// BEEP_CRITICAL: 3×80ms pulses, 40ms gaps, 200ms repeat interval
#define BEEP_CRITICAL_PULSE_MS  80
#define BEEP_CRITICAL_GAP_MS    40
#define BEEP_CRITICAL_PULSES     3
#define BEEP_CRITICAL_PERIOD_MS 200

// BEEP_WARNING: 1×300ms pulse, 700ms silence
#define BEEP_WARNING_PULSE_MS   300
#define BEEP_WARNING_PERIOD_MS 1000

// ── Display blink parameters ─────────────────────────────────────────────────

#define DISPLAY_BLINK_RED_MS     150     // fast blink: on+off period
#define DISPLAY_BLINK_YELLOW_MS  600     // slow blink: on+off period
