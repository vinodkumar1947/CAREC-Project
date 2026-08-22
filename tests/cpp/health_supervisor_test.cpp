#include <assert.h>
#include <stdint.h>

#include "health_supervisor.h"

int main() {
    assert(evaluate_system_health(true, 1000, 1000, 50).safe_to_operate);
    assert(evaluate_system_health(false, 1000, 1000, 50).reason ==
           HEALTH_DETECTOR_NOT_READY);
    assert(evaluate_system_health(true, 1000, 700, 50).reason ==
           HEALTH_DETECTOR_STALE);
    assert(evaluate_system_health(true, 1000, 1000, 250).reason ==
           HEALTH_LOOP_OVERRUN);
    const uint32_t near_rollover = UINT32_MAX - 25;
    assert(evaluate_system_health(true, 25, near_rollover, 50).safe_to_operate);
}
