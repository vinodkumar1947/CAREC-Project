# Preliminary Hazard Register

Status: Preliminary
Owner: Safety Workstream
Last Updated: 2026-08-23
Related Issues: CARE-036
Related ADRs: ADR-0003, ADR-0004

This is preliminary engineering analysis only. Severity, probability, and residual risk scales are not yet approved.

| ID | Hazard | Scenario | Severity | Probability | Risk | Mitigation | Verification | Status |
|---|---|---|---|---|---|---|---|---|
| HAZ-001 | Collision | Obstacle is missed or braking begins too late | TBD | TBD | TBD | Footprint-aware stop envelope; diverse sensing; speed limits | Fault-injected simulation and later controlled stopping tests | Open |
| HAZ-002 | Unexpected motion | Command is stale, corrupt, or issued during reset | TBD | TBD | TBD | Freshness checks; bounded commands; reset produces zero motion | Unit and integration tests | Open |
| HAZ-003 | Loss of sensing | Required range data freezes while moving | TBD | TBD | TBD | Per-source watchdog and conservative degraded state | Stale/dropout scenarios | Open |
| HAZ-004 | Localization failure | Pose jumps or becomes invalid during navigation | TBD | TBD | TBD | Confidence gate; cancel goal; safe stop | Localization fault injection | Open |
| HAZ-005 | E-stop failure | Stop is not latched or reset causes motion | TBD | TBD | TBD | Independent stop path; deliberate authorized reset | Unit, HIL, and controlled bench test | Open |
| HAZ-006 | Tip-over | Excess speed/turn on slope or threshold | TBD | TBD | TBD | Dynamics envelope; slope detection; speed/turn limits | Simulation then instrumented unoccupied tests | Open |
| HAZ-007 | Drop/stair event | Forward route contains an unobserved drop | TBD | TBD | TBD | Dedicated drop sensing; route constraints; safe stop | Edge-case simulation and controlled fixture | Open |
| HAZ-008 | AI misclassification | Person or doorway is incorrectly classified | TBD | TBD | TBD | Uncertainty; non-AI safety layer; diverse evaluation | Dataset slices and scenario tests | Open |
| HAZ-009 | Compute failure | Main process hangs while last command persists | TBD | TBD | TBD | Independent watchdog and command timeout | Kill/hang fault injection | Open |
| HAZ-010 | Battery/power failure | Brownout produces erratic control | TBD | TBD | TBD | Power monitoring; fail-safe controller behavior | Bench power-fault testing | Open |
| HAZ-011 | Motor-controller failure | Controller ignores or misinterprets safe command | TBD | TBD | TBD | Model-specific adapter, feedback, independent stop | HIL before unoccupied integration | Open |
| HAZ-012 | Unauthorized caregiver control | Remote actor issues a command | TBD | TBD | TBD | Authentication, consent, local priority, audit log | Security and integration tests | Open |
