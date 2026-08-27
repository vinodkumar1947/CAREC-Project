# Sensor Architecture

Status: Proposed
Owner: Sensor Fusion Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0003

Candidate sensing includes encoders, IMU, camera/depth, LiDAR or other ranging, and ultrasonic/drop sensors. Selection shall follow hazards, coverage, blind zones, update rate, latency, environment, mounting, power, privacy, and failure detectability—not novelty. Every source needs timestamps, coordinate frame, validity, covariance or confidence, health, calibration data, and a simulation/mock implementation.
