# Milestones

Status: Proposed
Owner: Project Maintainer
Last Updated: 2026-08-23
Related Issues: CARE-001 through CARE-015, CARE-040 through CARE-044
Related ADRs: ADR-0001, ADR-0002

## M0 — CAREC Simulation Foundation

**Goal:** A reproducible environment in which a contributor can run a virtual differential-drive wheelchair, provide motion commands, observe simulated sensor data, and verify basic safe-stop behavior.

Deliverables:

- documented development environment and one-command validation;
- ROS 2 workspace and package layout, subject to distribution ADR approval;
- selected simulator, subject to ADR approval;
- parameterized wheelchair URDF/SDF and differential-drive motion;
- keyboard/joystick teleoperation and bounded velocity commands;
- simulated odometry, IMU, and one range sensor;
- small indoor environment and versioned baseline scenarios;
- automated startup, headless smoke test, and contributor setup guide;
- command-timeout and emergency-stop simulation tests.

Exit criteria: a clean supported environment can launch, drive, observe sensors, trigger safety stop, and reproduce CI evidence without physical hardware. No physical deployment claim is implied.
