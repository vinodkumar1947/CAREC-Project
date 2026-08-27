# CAREC Software

Status: Proposed
Owner: Architecture Workstream
Last Updated: 2026-08-23
Related Issues: CARE-003, CARE-004
Related ADRs: ADR-0002

Planned hardware-independent application code is separated by responsibility: `ros2/`, `navigation/`, `perception/`, `ai/`, `sensor_fusion/`, and `common/`. Packages shall depend on documented interfaces and mocks. Existing runnable reference-simulation code remains in `autonomy_ws/` until migration is approved.
