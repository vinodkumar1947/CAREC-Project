# Level 1 — Manual Drive

Status: Proposed
Owner: Product Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: TBD

- **REQ-MAN-001:** Valid user input shall directly determine requested direction and speed within configured limits.
- **REQ-MAN-002:** Monitoring shall not unexpectedly alter user intent except through documented, testable safety intervention.
- **REQ-MAN-003:** Mode, interventions, faults, and command rejection shall be perceivable through accessible feedback.
- **REQ-MAN-004:** Loss or staleness of the input signal shall produce the defined safe state.

Acceptance evidence begins in simulation with command, timeout, limit, and mode-transition tests.
