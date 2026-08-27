# CAREC Roadmap

Status: Proposed

Owner: Project Maintainer

Last Updated: 2026-08-23

Related Issues: [Initial backlog](docs/09-project-management/initial-backlog.md)

Related ADRs: [Decision index](docs/11-decisions/README.md)

No completion percentages are assigned without evidence. Advancement requires documented exit criteria and safety review.

## Phase 0 — Foundation

Requirements, architecture, repository structure, simulation-platform selection, contributor workflow, and preliminary safety framework. **Current phase.**

## Phase 1 — Digital Wheelchair

Parameterized differential-drive model, motor behavior, encoders, IMU, simulated range sensors, and indoor world.

## Phase 2 — Manual Simulation

Joystick/keyboard input, teleoperation, velocity control, diagnostics, command timeout, and emergency stop.

## Phase 3 — Assisted Driving

Collision avoidance, speed assistance, doorway support, path correction, safety zones, and transparent intervention.

## Phase 4 — Autonomous Navigation

Mapping, SLAM, localization, Nav2 or approved alternative, goal navigation, dynamic avoidance, and recovery.

## Phase 5 — AI Perception

People and doorway detection, scene understanding, uncertainty, semantic navigation research, and diverse evaluation.

## Phase 6 — Hardware Prototype

Approved compute, sensors, motor interface, independent safety controller, power architecture, and unoccupied bench validation.

## Phase 7 — Hardware-in-the-Loop

Real controllers/sensors against simulated scenarios, timing, watchdog, fault, and interface evidence.

## Phase 8 — Controlled Wheelchair Testing

Owner-approved, unoccupied controlled testing followed only by separately authorized supervised user validation. Simulation results never substitute for these gates.

The recommended first milestone is [M0 — CAREC Simulation Foundation](docs/09-project-management/milestones.md). Earlier twelve-week planning is retained in git history and the [migration report](docs/documentation-migration-report.md); dates must be re-baselined by the maintainer.
