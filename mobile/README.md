# CAREC User Interface Research

> **No active mobile application is implemented.** This directory formerly described a SenseCraft caregiver-app concept. That plan is retained in Git history and is not a CAREC Sim 0.1 commitment.

## Current direction

The first user interface is simulator-facing and must support:

- user intent without reducing user agency;
- explicit emergency stop and reset state;
- visible reasons for slowdown, stop, or rejected commands;
- navigation destination selection;
- system, sensor, and localization health;
- replayable safety events; and
- accessible interaction reviewed with wheelchair users and relevant professionals.

The interface must not imply that simulation results make the system safe for occupied-wheelchair use.

## Not currently promised

- SenseCraft Mate compatibility
- caregiver push notifications
- Home Assistant or Node-RED integration
- cloud accounts or cloud-required operation
- BLE-based location tracking
- remote motor control

Any future caregiver or clinician view requires a separate privacy and consent design. Personal health information, identifiable participant data, credentials, and raw private recordings must not be committed to this repository.

## Contributor path

Accessibility and interface work should start from a scoped GitHub issue with:

- the intended user and decision;
- an accessible interaction requirement;
- mock or simulated data only;
- objective acceptance criteria;
- privacy impact; and
- safety classification.

See the [project aim](../docs/vision/PROJECT_AIM.md), [team workflow](../docs/contributors/TEAM_WORKFLOW.md), and issues labeled [`accessibility`](https://github.com/vinodkumar1947/CAREC-Project/issues?q=is%3Aissue+is%3Aopen+label%3Aaccessibility).
