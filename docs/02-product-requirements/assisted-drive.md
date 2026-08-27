# Level 2 — Assisted Drive

Status: Proposed
Owner: Product Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: TBD

Assisted drive retains continuous user intent while applying transparent constraints such as collision avoidance, doorway alignment, speed limiting, edge/drop detection, path correction, assisted turning, and safe stop.

- **REQ-AST-001:** Assistance shall declare why and how a command was modified.
- **REQ-AST-002:** The user shall be able to return to manual mode through a defined, safe transition.
- **REQ-AST-003:** Uncertain or stale assistance inputs shall not produce increased motion authority.
- **REQ-AST-004:** Intervention thresholds and nuisance interventions shall be measured in representative scenarios.
