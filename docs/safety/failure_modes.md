# CAREC Hazard Log

This is a living engineering risk record governed by `SAFETY_PLAN.md`. Risk
ratings are provisional until severity/probability definitions and appropriate
domain review are approved. No entry authorizes occupied use.

| ID | Hazardous situation / possible harm | Initial concern | Control requirement | Verification | Residual status |
|---|---|---|---|---|---|
| HAZ-001 | Obstacle is missed and motion continues; collision/injury | Critical | REQ-SAFE-004, sensor coverage and ODD limits | TST-SAFE-004 plus future perception suite | Open |
| HAZ-002 | Stale command continues motion; collision | Critical | REQ-SAFE-001 | TST-SAFE-001 | Controlled in reference sim; physical open |
| HAZ-003 | Stale sensor is treated as clear; collision | Critical | REQ-SAFE-002 | TST-SAFE-002 | Controlled in reference sim; physical open |
| HAZ-004 | Invalid localization permits navigation | Critical | REQ-SAFE-003 | TST-SAFE-003 | Controlled in reference sim; physical open |
| HAZ-005 | E-stop clears without deliberate authorization | Critical | REQ-SAFE-005 | TST-SAFE-005 | Controlled in reference sim; physical open |
| HAZ-006 | Excessive requested velocity causes unstable or late stop | Critical | REQ-SAFE-006 | TST-SAFE-006 | Controlled in reference sim; profile validation open |
| HAZ-007 | Software freeze or loop overrun removes warning/control | Critical | REQ-FW-001 | TST-FW-001 | Partial; hardware watchdog open |
| HAZ-008 | Update starts during operation or malicious firmware loads | Critical | REQ-CYB-001 | Configuration review | OTA disabled; secure update open |
| HAZ-009 | Unauthorized BLE command changes safety behavior | Critical | REQ-CYB-002 | Interface review | Write command removed; authenticated design open |
| HAZ-010 | Camera model returns no recognized object and path is treated clear | Critical | REQ-PER-001 | Future negative-obstacle and unknown-object suite | Open; monocular warning only |
| HAZ-011 | Cliff/stair/drop-off is outside sensor capability | Critical | REQ-PER-002 | Future dedicated scenario/sensor tests | Open |
| HAZ-012 | Adapter mismatch or electrical fault commands unintended motion | Critical | REQ-ADP-001 | Per-model bench validation | Open; community hardware prohibited |
| HAZ-013 | Mount/cable detaches or entangles chair | Critical | REQ-HW-001 | Mechanical inspection and bench protocol | Open |
| HAZ-014 | Alerts cause distress, confusion or alert fatigue | High | REQ-HFE-001 | User-centered formative evaluation | Open |
| HAZ-015 | Telemetry exposes user behavior or identity | High | REQ-PRV-001 | Privacy/security review | Open |
| HAZ-016 | Simulation behavior does not transfer to physical dynamics | Critical | REQ-VV-001 | Model correlation and unoccupied bench tests | Open |

Historical battery, mounting, motion-gate and residual-risk claims were removed
because measured evidence was not present. New hazards must be added whenever a
change introduces a new energy source, command path, sensor, user interaction,
environment or external dependency.
