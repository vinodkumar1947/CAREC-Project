#pragma once

#include <stdint.h>

enum SystemHealthReason {
    HEALTH_OK = 0,
    HEALTH_DETECTOR_NOT_READY,
    HEALTH_DETECTOR_STALE,
    HEALTH_LOOP_OVERRUN,
};

struct SystemHealth {
    bool safe_to_operate;
    SystemHealthReason reason;
};

// Portable health boundary. Timestamp subtraction is intentionally unsigned
// so the comparison remains correct across the uint32_t millisecond rollover.
inline SystemHealth evaluate_system_health(bool detector_ready,
                                           uint32_t now_ms,
                                           uint32_t detector_stamp_ms,
                                           uint32_t loop_duration_ms,
                                           uint32_t detector_timeout_ms = 250,
                                           uint32_t loop_deadline_ms = 200) {
    if (!detector_ready) return {false, HEALTH_DETECTOR_NOT_READY};
    if (now_ms - detector_stamp_ms > detector_timeout_ms)
        return {false, HEALTH_DETECTOR_STALE};
    if (loop_duration_ms > loop_deadline_ms)
        return {false, HEALTH_LOOP_OVERRUN};
    return {true, HEALTH_OK};
}
