# Simulation Test Cases

Status: Draft
Owner: Testing Workstream
Last Updated: 2026-08-23
Related Issues: CARE-012, CARE-040
Related ADRs: TBD

| ID | Scenario | Expected evidence |
|---|---|---|
| SIM-001 | Spawn at rest | Stable model, valid TF and zero motion |
| SIM-002 | Manual straight/turn command | Bounded motion and odometry |
| SIM-003 | Command timeout | Deterministic safe stop and reason |
| SIM-004 | Obstacle inside stop zone | Forward command rejected; clearance logged |
| SIM-005 | Stale required sensor | Safe stop; diagnostic identifies source |
| SIM-006 | E-stop and reset | Stop latches; reset creates no motion |
| SIM-007 | Doorway traversal | No collision; clearance and completion logged |
| SIM-008 | Localization loss | Navigation cancels or safely stops |

Thresholds are TBD until dynamics and risk controls are approved.
