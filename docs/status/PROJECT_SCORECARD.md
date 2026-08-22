# Project Scorecard

**Status date:** August 22, 2026

**Current milestone:** Contribution foundation

**Overall simulation-autonomy program:** 0% earned

The existing firmware prototype is tracked separately because it does not satisfy the new simulation-autonomy milestones.

## Earned progress

| Deliverable | Weight | Earned | Evidence |
|---|---:|---:|---|
| Reproducible contributor setup | 10% | 0% | Not implemented |
| Digital wheelchair and sensors | 15% | 0% | Not implemented |
| Safety kernel | 20% | 0% | Not implemented |
| Scenario and metrics framework | 15% | 0% | Not implemented |
| Shared control | 15% | 0% | Not implemented |
| Supervised navigation | 10% | 0% | Not implemented |
| Perception research | 5% | 0% | New benchmark not established |
| Hardware-in-the-loop validation | 5% | 0% | Deferred |
| Controlled physical validation | 5% | 0% | Deferred |

## Repository evidence

| Signal | Current state |
|---|---|
| Autonomy build | Not configured |
| Simulation regression | Not configured |
| Safety scenarios | 0 defined in new framework |
| Open safety blockers | Failure-state semantics; hardware boundary implementation |
| Latest release | No simulation-first release |

## Dashboard automation plan

GitHub Actions will generate this scorecard and a machine-readable `status.json` from merged milestone evidence, tests, and scenario reports. GitHub Pages will present delivery, engineering health, autonomy safety metrics, and contribution activity. Individual contributors will not be ranked by commits or lines of code.

A milestone receives credit only after its acceptance evidence is merged. Percentages are never inferred from time spent or number of issues opened.
