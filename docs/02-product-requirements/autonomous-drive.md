# Level 3 — Autonomous Drive

Status: Proposed
Owner: Navigation Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0006

Autonomous mode targets supervised indoor commands such as “take me to the kitchen,” classroom, or bedroom. It requires mapping, localization, global and local planning, dynamic obstacle handling, goal confirmation, cancellation, and recovery.

- **REQ-AUT-001:** Autonomous motion shall operate only within a documented ODD.
- **REQ-AUT-002:** A user or caregiver stop shall preempt navigation.
- **REQ-AUT-003:** Invalid localization, route failure, or unsafe perception shall transition to a defined safe state.
- **REQ-AUT-004:** Mode changes and destinations shall require unambiguous confirmation.

This capability must progress through repeatable simulation, controlled testing, and human-supervised validation. It is not currently implemented or approved for physical use.
