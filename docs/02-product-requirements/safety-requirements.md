# Safety Requirements

Status: Draft
Owner: Safety Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0004

All motion commands shall pass through an independent safety supervisor. It shall reject stale commands, enforce verified limits, react deterministically to required-sensor or compute failure, honor manual override and emergency stop, and expose machine-readable fault reasons. Safety controls shall be traceable from [hazards](../06-safety/risk-register.md) to requirements and tests. AI output may inform behavior but shall not be the sole authority for safety-critical actuation.
