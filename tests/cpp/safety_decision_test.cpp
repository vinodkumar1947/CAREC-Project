#include <assert.h>

#include "safety_decision.h"

int main() {
    assert(decide_safety_zone(DETECTION_OK, 30.0f).zone == ZONE_RED);
    assert(decide_safety_zone(DETECTION_OK, 60.0f).zone == ZONE_YELLOW);
    assert(decide_safety_zone(DETECTION_OK, 100.0f).zone == ZONE_GREEN);

    const SafetyDecision empty_frame = decide_safety_zone(DETECTION_OK, 200.0f);
    assert(empty_frame.zone == ZONE_GREEN);
    assert(!empty_frame.detector_degraded);

    const SafetyDecision not_ready = decide_safety_zone(DETECTION_NOT_READY, 200.0f);
    assert(not_ready.zone == ZONE_RED);
    assert(not_ready.distance_cm == 0.0f);
    assert(not_ready.detector_degraded);

    const SafetyDecision inference_failed =
        decide_safety_zone(DETECTION_INFERENCE_FAILED, 200.0f);
    assert(inference_failed.zone == ZONE_RED);
    assert(inference_failed.detector_degraded);
}
