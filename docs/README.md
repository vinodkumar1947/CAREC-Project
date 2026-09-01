# CAREC Documentation Home

Status: Draft

Owner: Project Maintainer

Last Updated: 2026-09-01

Related ADRs: [Decision index](11-decisions/README.md)

CAREC is an early-stage, **simulation-first ROS 2 autonomous wheelchair** project. Current engineering work focuses on the digital wheelchair, ROS 2 interfaces, manual/shared control, localization, navigation, perception, safety supervision, deterministic scenarios, and evidence-driven validation.

Physical wheelchair hardware, embedded firmware, ESP-IDF, and device-specific prototypes are **not part of the current CAREC Sim milestone**.

This repository is the engineering source of truth for the published CAREC GitBook. GitBook navigation is defined by [`SUMMARY.md`](SUMMARY.md); approved documentation changes originate through Git and follow the pull-request workflow.

## Start Here

- [Vision](01-project-overview/vision.md), [goals](01-project-overview/goals.md), and [non-goals](01-project-overview/non-goals.md)
- [Getting started](08-contributors/getting-started.md) and [first task](08-contributors/first-task.md)
- [Roadmap](../ROADMAP.md) and [milestones](09-project-management/milestones.md)
- [Project scorecard](status/PROJECT_SCORECARD.md)

## Architecture

- [Architecture index](03-system-architecture/README.md)
- [High-level architecture](03-system-architecture/high-level-architecture.md)
- [ROS 2 architecture](03-system-architecture/ros2-architecture.md)
- [Interfaces](03-system-architecture/interfaces.md)
- [Data flow](03-system-architecture/data-flow.md)
- [Architecture decisions](11-decisions/README.md)

## Simulation and Autonomy

- [Simulation overview](04-simulation/README.md)
- [Simulation strategy](04-simulation/simulation-strategy.md)
- [Wheelchair model](04-simulation/wheelchair-model.md)
- [Sensor simulation](04-simulation/sensor-simulation.md)
- [Environment models](04-simulation/environment-models.md)
- [Simulation test cases](04-simulation/simulation-test-cases.md)
- [Autonomous navigation](04-simulation/autonomous-navigation.md)
- [ROS 2 engineering](05-engineering/ros2.md)
- [Navigation](05-engineering/navigation.md)
- [Computer vision](05-engineering/computer-vision.md)
- [AI/ML](05-engineering/ai-ml.md)
- [Sensor fusion](05-engineering/sensor-fusion.md)

## Safety and Testing

- [Safety overview](06-safety/README.md)
- [Safety philosophy](06-safety/safety-philosophy.md)
- [Hazard analysis](06-safety/hazard-analysis.md)
- [Failure modes](06-safety/failure-modes.md)
- [Safety validation](06-safety/safety-validation.md)
- [Testing strategy](07-testing/testing-strategy.md)
- [Simulation testing](07-testing/simulation-testing.md)

## Contributors and Project Management

- [Development environment](08-contributors/development-environment.md)
- [Git workflow](08-contributors/git-workflow.md)
- [Issue workflow](08-contributors/issue-workflow.md)
- [PR process](08-contributors/pull-request-process.md)
- [Initial backlog](09-project-management/initial-backlog.md)
- [Progress tracking](09-project-management/progress-tracking.md)
- [Weekly process](09-project-management/weekly-process.md)

## Source-of-truth rule

GitHub `main` is authoritative. GitHub Issues describe execution state. GitBook publishes approved documentation. Discord is coordination-only. READMEs should point to canonical status rather than duplicate it.

- Machine status: [`../evaluation/status.json`](../evaluation/status.json)
- Human status: [`status/PROJECT_SCORECARD.md`](status/PROJECT_SCORECARD.md)
- ADRs: [`11-decisions/`](11-decisions/)
