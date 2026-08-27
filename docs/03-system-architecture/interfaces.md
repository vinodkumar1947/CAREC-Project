# Interface Principles

Status: Proposed
Owner: Architecture Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0004, ADR-0008

Each interface specification shall define producer, consumer, units, coordinate frame, valid range, rate, timestamp, timeout, QoS, version, health, authentication where applicable, safe default, and test double. Hardware-independent code shall depend on the safe-motion and sensor contracts rather than vendor drivers. Breaking changes require an ADR or explicit compatibility plan.

Candidate core messages are `UserIntent`, `SystemMode`, `MotionProposal`, `SafetyState`, `SafeMotionCommand`, `SensorHealth`, and `PlatformStatus`; names and schemas are TBD.
