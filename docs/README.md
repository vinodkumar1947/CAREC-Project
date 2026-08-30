# CAREC Documentation Home

Status: Draft

Owner: Project Maintainer

Last Updated: 2026-09-23

Related Issues: TBD

Related ADRs: [Decision index](11-decisions/README.md)

This is the engineering entry point for CAREC. CAREC is an early-stage, simulation-first intelligent-wheelchair research project. Documents marked **Proposed** describe direction, not completed capability or approved hardware.

This repository is the engineering source of truth for the published
[CAREC GitBook](https://carec.gitbook.io/carec-docs). GitBook navigation is
defined by [`SUMMARY.md`](SUMMARY.md); changes should originate through Git and
be reviewed with the same pull-request process as code.

## Start Here

- [Vision](01-project-overview/vision.md), [goals](01-project-overview/goals.md), and [non-goals](01-project-overview/non-goals.md)
- [Getting started](08-contributors/getting-started.md) and [first task](08-contributors/first-task.md)
- [Roadmap](../ROADMAP.md) and [M0 milestone](09-project-management/milestones.md)
- [Migration report](documentation-migration-report.md)

## Product

- [Product requirements](02-product-requirements/README.md)
- [Manual](02-product-requirements/manual-drive.md), [assisted](02-product-requirements/assisted-drive.md), and [autonomous](02-product-requirements/autonomous-drive.md) driving
- [Accessibility](02-product-requirements/accessibility.md) and [safety requirements](02-product-requirements/safety-requirements.md)

## Architecture

- [Architecture index](03-system-architecture/README.md)
- [High-level architecture](03-system-architecture/high-level-architecture.md), [software](03-system-architecture/software-architecture.md), [data flow](03-system-architecture/data-flow.md), and [interfaces](03-system-architecture/interfaces.md)

## Simulation

- [Simulation strategy](04-simulation/simulation-strategy.md), [platform selection](04-simulation/simulator-selection.md), and [test cases](04-simulation/simulation-test-cases.md)

## Engineering

- [ROS 2](05-engineering/ros2.md), [navigation](05-engineering/navigation.md), [vision](05-engineering/computer-vision.md), [AI/ML](05-engineering/ai-ml.md), [sensor fusion](05-engineering/sensor-fusion.md)
- [Firmware](05-engineering/firmware.md), [hardware](05-engineering/hardware.md), [mobile](05-engineering/mobile-app.md), and [cloud](05-engineering/cloud.md)

## Safety

- [Safety philosophy](06-safety/safety-philosophy.md), [hazard analysis](06-safety/hazard-analysis.md), [risk register](06-safety/risk-register.md), and [validation](06-safety/safety-validation.md)

## Testing

- [Testing strategy](07-testing/testing-strategy.md) and the individual test-level guides in `07-testing/`

## Contributors

- [Development environment](08-contributors/development-environment.md), [Git workflow](08-contributors/git-workflow.md), [issue workflow](08-contributors/issue-workflow.md), and [PR process](08-contributors/pull-request-process.md)

## Project Management

- [Initial backlog](09-project-management/initial-backlog.md), [weekly process](09-project-management/weekly-process.md), [progress tracking](09-project-management/progress-tracking.md), and [release process](09-project-management/release-process.md)

## Meetings, Decisions, and Research

- [Meetings](10-meetings/README.md)
- [Architecture decisions](11-decisions/README.md)
- [Research index](12-research/README.md)

## Preserved Records

Earlier documents remain under `docs/architecture`, `docs/specifications`, `docs/safety`, `docs/testing`, `docs/guides`, and `docs/archive`. Their historical claims do not supersede the current roadmap, requirements, safety boundaries, or approved ADRs.
