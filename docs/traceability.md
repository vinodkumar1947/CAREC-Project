# Requirements and Verification Traceability

| Requirement | Statement | Hazard | Verification evidence | Status |
|---|---|---|---|---|
| REQ-SAFE-001 | A command older than 250 ms produces zero motion | HAZ-002 | TST-SAFE-001 `test_faults_deterministically_stop[COMMAND_STALE]` | Implemented in reference sim |
| REQ-SAFE-002 | Required sensor data older than 250 ms produces zero motion | HAZ-003 | TST-SAFE-002 `test_faults_deterministically_stop[SENSOR_STALE]` | Implemented in reference sim |
| REQ-SAFE-003 | Invalid localization produces zero motion | HAZ-004 | TST-SAFE-003 `test_faults_deterministically_stop[LOCALIZATION_INVALID]` | Implemented in reference sim |
| REQ-SAFE-004 | A forward obstacle inside 0.6 m produces zero forward motion | HAZ-001 | TST-SAFE-004 `test_faults_deterministically_stop[OBSTACLE_STOP]` | Implemented in reference sim |
| REQ-SAFE-005 | E-stop remains latched until physically authorized reset | HAZ-005 | TST-SAFE-005 `test_emergency_stop_latches_until_authorized_reset` | Implemented in reference sim |
| REQ-SAFE-006 | Requests are bounded to configured linear/angular limits | HAZ-006 | TST-SAFE-006 `test_commands_are_limited` | Implemented in reference sim |
| REQ-FW-001 | Detector failure or processing-loop overrun reports degraded health; future timestamped producers must reject stale results | HAZ-007 | TST-FW-001 `health_supervisor_test.cpp` | Portable logic and prior-cycle timing implemented; producer timestamps open |
| REQ-CYB-001 | Operational builds cannot enter network OTA | HAZ-008 | `FEATURE_WIFI_OTA == 0`; CI review | Implemented by default |
| REQ-CYB-002 | BLE exposes no unauthenticated command characteristic | HAZ-009 | Source/interface review | Implemented |
| REQ-ADP-001 | Physical adapters are model-specific and accept safety output only | HAZ-012 | Architecture review | Specified; physical verification open |

Names are stable evidence identifiers. Pull requests changing these behaviors
must update this table, relevant hazards, and tests in the same change.
