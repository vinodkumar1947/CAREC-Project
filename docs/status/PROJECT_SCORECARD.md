# Project Scorecard

**Status date:** August 22, 2026

**Current milestone:** Reference safety foundation

**Overall simulation-autonomy program:** 5% earned

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

## Repository evidence

| Signal | Current state |
|---|---|
| Autonomy build | Dependency-free reference model; ROS 2 not yet configured |
| Simulation regression | Reference safety regression configured in CI |
| Safety scenarios | 6 baseline fault/behavior scenarios |
| Open safety blockers | ROS safety node, collision geometry, physical safety controller and adapter validation |
| Latest release | No simulation-first release |

## Dashboard foundation

The machine-readable source is [`evaluation/status.json`](../../evaluation/status.json), and CI validates its schema. Requirements, hazards, tests and scenario references are also checked by `scripts/validate_traceability.py`. Automatic metric collection and a GitHub Pages presentation remain planned work. Individual contributors will not be ranked by commits or lines of code.

A milestone receives credit only after its acceptance evidence is merged. Percentages are never inferred from time spent or number of issues opened.
