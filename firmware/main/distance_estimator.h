#pragma once
#include <Arduino.h>
#include "sensecraft_detection.h"

// Monocular distance estimation — bounding-box-width heuristic
//
// Formula:  distance_cm = (real_width_cm * focal_length_px) / bbox_width_px
//
// With SenseCAP Watcher W1-A (120° horizontal FOV, inference at 640px width):
//   focal_length_px = (image_width / 2) / tan(FOV_rad / 2)
//                   = 320 / tan(60°) ≈ 184.75 px
//
// Since SenseCraft returns normalised bbox_w (0–1):
//   bbox_width_px = bbox_w_norm * 640
//   distance_cm = real_width_cm / (bbox_w_norm * 2 * tan(60°))
//               = real_width_cm / (bbox_w_norm * 3.464)
//
// Calibration check (person, real_width ≈ 50cm):
//   bbox_w = 0.241 → dist ≈ 60cm   (DIST_RED boundary)   ✓
//   bbox_w = 0.144 → dist ≈ 100cm  (DIST_YELLOW boundary) ✓
//   bbox_w = 0.289 → dist ≈ 50cm   (mid RED zone)
//   bbox_w = 0.180 → dist ≈ 80cm   (mid YELLOW zone)
//
// LIMITATION: monocular camera → estimated distance, not measured depth.
// Accuracy degrades for thin objects (door frame), very close (<15cm), or
// objects whose real width differs from the reference. Field calibration
// (tape measure at 60cm and 100cm) is required before deployment.

// ---- Camera parameters (SenseCAP Watcher W1-A) ----------------------------
#define CAMERA_FOV_DEG        120.0f
#define CAMERA_INFERENCE_W_PX 640

// tan(FOV/2) for 120° = tan(60°) = 1.7321
#define TAN_HALF_FOV  1.7321f

// ---- Reference object widths (cm) -----------------------------------------
// Lateral width as seen by a forward-facing camera at wheelchair height.
// Err on the side of a SMALLER width → shorter estimated distance → earlier warning.
// Adjust after field calibration with calibrate_distance.py.
#define REF_WIDTH_PERSON_CM    50.0f   // shoulder width
#define REF_WIDTH_CHAIR_CM     45.0f   // seat width
#define REF_WIDTH_COUCH_CM    150.0f   // typical 2–3 seat sofa
#define REF_WIDTH_TABLE_CM     80.0f   // typical dining / side table
#define REF_WIDTH_BED_CM      100.0f   // single/twin bed width
#define REF_WIDTH_BENCH_CM     80.0f
#define REF_WIDTH_DOOR_CM      80.0f
#define REF_WIDTH_BICYCLE_CM   55.0f   // handlebar span
#define REF_WIDTH_DOG_CM       40.0f   // medium dog body width
#define REF_WIDTH_CAT_CM       20.0f
#define REF_WIDTH_SUITCASE_CM  40.0f
#define REF_WIDTH_BACKPACK_CM  30.0f
#define REF_WIDTH_BALL_CM      20.0f   // typical play ball
#define REF_WIDTH_DEFAULT_CM   45.0f   // conservative fallback — smaller → closer estimate

// ---- Clip range -----------------------------------------------------------
// Distances outside this range are likely estimation errors; treat as clear.
#define DIST_MIN_CM   10.0f
#define DIST_MAX_CM  200.0f

// ---- Calibration scale factor ---------------------------------------------
// Multiply estimated distance by this after field calibration.
// Default 1.0 (no correction). Tune by placing known object at 50cm and
// adjusting until estimate matches.
#define DIST_CALIBRATION_SCALE 1.0f

// ---------------------------------------------------------------------------

static float reference_width_for(const char* label) {
    if (strstr(label, "person") || strstr(label, "human"))    return REF_WIDTH_PERSON_CM;
    if (strstr(label, "couch")  || strstr(label, "sofa"))     return REF_WIDTH_COUCH_CM;
    if (strstr(label, "chair")  || strstr(label, "stool"))    return REF_WIDTH_CHAIR_CM;
    if (strstr(label, "table")  || strstr(label, "desk"))     return REF_WIDTH_TABLE_CM;
    if (strstr(label, "bed"))                                  return REF_WIDTH_BED_CM;
    if (strstr(label, "bench"))                                return REF_WIDTH_BENCH_CM;
    if (strstr(label, "door"))                                 return REF_WIDTH_DOOR_CM;
    if (strstr(label, "bicycle") || strstr(label, "bike"))    return REF_WIDTH_BICYCLE_CM;
    if (strstr(label, "dog"))                                  return REF_WIDTH_DOG_CM;
    if (strstr(label, "cat"))                                  return REF_WIDTH_CAT_CM;
    if (strstr(label, "suitcase"))                             return REF_WIDTH_SUITCASE_CM;
    if (strstr(label, "backpack") || strstr(label, "bag"))    return REF_WIDTH_BACKPACK_CM;
    if (strstr(label, "ball") || strstr(label, "teddy"))      return REF_WIDTH_BALL_CM;
    return REF_WIDTH_DEFAULT_CM;
}

// Estimate distance to a single detected object from its normalised bbox width.
// Returns DIST_MAX_CM if bbox_w is too small to be reliable.
static float estimate_distance_cm(const Detection* d) {
    if (d->bbox_w < 0.02f) return DIST_MAX_CM;  // too narrow to trust

    float real_w = reference_width_for(d->label);
    float dist   = real_w / (d->bbox_w * 2.0f * TAN_HALF_FOV);
    dist        *= DIST_CALIBRATION_SCALE;

    if (dist < DIST_MIN_CM) dist = DIST_MIN_CM;
    if (dist > DIST_MAX_CM) dist = DIST_MAX_CM;

    return dist;
}

// Pick the nearest obstacle from a full detection result.
// Returns DIST_MAX_CM (clear) if nothing is above confidence threshold.
static float nearest_obstacle_cm(DetectionResult* result) {
    float nearest = DIST_MAX_CM;

    for (int i = 0; i < result->count; i++) {
        Detection* d = &result->items[i];
        if (d->confidence < DETECTION_CONFIDENCE_THRESHOLD) continue;

        float dist = estimate_distance_cm(d);
        if (dist < nearest) nearest = dist;
    }
    return nearest;
}

// Print distance estimate for each detection (debug).
static void log_distances(DetectionResult* result) {
    for (int i = 0; i < result->count; i++) {
        Detection* d = &result->items[i];
        if (d->confidence < DETECTION_CONFIDENCE_THRESHOLD) continue;
        float dist = estimate_distance_cm(d);
        Serial.printf("[Distance] %-12s conf=%.2f  bbox_w=%.3f  est=%.1fcm\n",
                      d->label, d->confidence, d->bbox_w, dist);
    }
}
