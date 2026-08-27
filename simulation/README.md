# CAREC Simulation Workspace

Status: Proposed
Owner: Simulation Workstream
Last Updated: 2026-08-23
Related Issues: CARE-004 through CARE-012
Related ADRs: ADR-0001, ADR-0002

This directory is the planned home for simulator-specific assets:

- `models/`: wheelchair and reusable object models
- `environments/`: versioned indoor worlds
- `sensors/`: sensor configuration and fault models
- `scenarios/`: machine-readable scenario definitions
- `tests/`: simulation launch and acceptance tests

The existing dependency-free reference simulator remains in [`../autonomy_ws/carec_simulation`](../autonomy_ws/carec_simulation) until the ROS 2/simulator ADRs are approved and migration is planned.
