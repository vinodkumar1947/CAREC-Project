#pragma once

#include "../config/zone_config.h"

enum DetectionStatus {
    DETECTION_OK = 0,
    DETECTION_NOT_READY,
    DETECTION_INFERENCE_FAILED,
};

struct SafetyDecision {
    float distance_cm;
    int zone;
    bool detector_degraded;
};

// Portable, deterministic safety boundary. This header intentionally has no
// Arduino or ESP-IDF dependencies so the production decision can be compiled
// and tested on contributor machines and in CI.
inline SafetyDecision decide_safety_zone(DetectionStatus status, float distance_cm) {
    if (status != DETECTION_OK) {
        return {0.0f, ZONE_RED, true};
    }

    int zone;
    if (distance_cm < DIST_RED) {
        zone = ZONE_RED;
    } else if (distance_cm < DIST_YELLOW) {
        zone = ZONE_YELLOW;
    } else {
        zone = ZONE_GREEN;
    }
    return {distance_cm, zone, false};
}
