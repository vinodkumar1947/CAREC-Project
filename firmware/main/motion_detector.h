#pragma once
#include <Arduino.h>

// Motion detector — SenseCAP Watcher W1-A
//
// Optical flow (Lucas-Kanade) is NOT implementable on the Watcher because the
// OV5647 camera feeds the Himax HX6538 NPU directly over MIPI. The ESP32-S3
// never sees raw frames — only SSCMA bounding-box results.
//
// Phase 1 strategy (Option A): treat the wheelchair as always moving forward.
// This runs detection every loop, which is safe and simpler.
// Detection still stops at ZONE_GREEN (>100 cm) via the main loop — the motion
// gate just no longer suppresses it at STATIONARY.
//
// Phase 2 plan: add Grove ADXL345 accelerometer to the IIC port (FEATURE_ACCEL_GATE
// in build_flags.h). When FEATURE_ACCEL_GATE=1 and the accelerometer reads below
// the stillness threshold, suppress alerts to reduce false positives while parked.

typedef enum {
    MOTION_STATIONARY = 0,
    MOTION_FORWARD    = 1,
    MOTION_BACKWARD   = 2,
} MotionState;

inline void motion_init() {
    Serial.println("[Motion] Phase 1: always-forward (optical flow not available on Watcher)");
}

inline MotionState motion_update()                                     { return MOTION_FORWARD; }
inline MotionState motion_update_gray(const uint8_t*)                  { return MOTION_FORWARD; }
inline MotionState motion_update_rgb565(const uint8_t*, int, int)      { return MOTION_FORWARD; }
inline MotionState motion_state()                                      { return MOTION_FORWARD; }
inline bool        motion_is_active()                                  { return true; }
inline bool        motion_is_forward()                                 { return true; }
inline bool        motion_is_backward()                                { return false; }
inline void        motion_log()                                        {}
