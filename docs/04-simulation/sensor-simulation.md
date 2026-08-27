# Sensor Simulation

Status: Proposed
Owner: Simulation and Sensor Fusion Workstreams
Last Updated: 2026-08-23
Related Issues: CARE-009, CARE-010
Related ADRs: ADR-0003

Initial virtual sensors should include encoders/odometry, IMU, and at least one range source. Camera, LiDAR, ultrasonic, and drop sensing follow hazard and architecture review. Models should support configurable noise, bias, rate, latency, dropout, stale data, obstruction, and frame errors so failure handling can be tested rather than assumed.
