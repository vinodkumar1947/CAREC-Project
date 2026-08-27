# Obstacle Avoidance

Status: Proposed
Owner: Navigation and Safety Workstreams
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0003, ADR-0006

- Detect obstacles relevant to the configured footprint and stopping envelope.
- Reduce or reject motion before the verified stopping boundary is crossed.
- Treat stale, missing, contradictory, or out-of-range sensing explicitly.
- Record detection, intervention, clearance, latency, and false-stop evidence.
- Never equate a vision classification alone with a safety-certified distance measurement.

Thresholds are TBD pending dynamics, sensor, and risk evidence.
