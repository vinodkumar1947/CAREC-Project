# Project Scorecard

**Status date:** 2026-08-22

**Current milestone:** Reference Safety Foundation

**Overall simulation-autonomy program:** 5% earned

This scorecard is generated from `evaluation/status.json` and checked-in test and scenario evidence.

The existing firmware prototype is tracked separately because it does not satisfy the new simulation-autonomy milestones.

## Earned progress

| Deliverable | Weight | Earned | Evidence |
|---|---:|---:|---|
| Reproducible contributor setup | 10% | 3% | Container specified; full clean build still pending |
| Digital wheelchair and sensors | 15% | 0% | Not implemented |
| Safety kernel | 20% | 1% | Dependency-free executable reference contract and fault tests |
| Scenario and metrics framework | 15% | 1% | Six baseline scenario IDs and CI regression; metrics/reporting pending |
| Shared control | 15% | 0% | Not implemented |
| Supervised navigation | 10% | 0% | Not implemented |
| Perception research | 5% | 0% | New benchmark not established |
| Hardware-in-the-loop validation | 5% | 0% | Deferred |
| Controlled physical validation | 5% | 0% | Deferred |

## Test state

- **Simulation regression:** `configured-reference-model`
- **Checked-in test files:** 1
- **Test functions discovered:** 6
- **Execution result:** A passing CI result is not stored in the checked-in test evidence; consult the latest CI run.

## Safety scenarios

**Baseline scenarios:** 6

| ID | Scenario | Expected result |
|---|---|---|
| SCN-001 | clear corridor | `OK` |
| SCN-002 | forward obstacle | `OBSTACLE_STOP` |
| SCN-003 | stale command | `COMMAND_STALE` |
| SCN-004 | stale sensor | `SENSOR_STALE` |
| SCN-005 | invalid localization | `LOCALIZATION_INVALID` |
| SCN-006 | latched emergency stop | `ESTOP_LATCHED` |

## Release

**Latest release:** No simulation-first release recorded

---

Progress reflects accepted deliverables and evidence, not contributor activity, commits, or lines of code.
