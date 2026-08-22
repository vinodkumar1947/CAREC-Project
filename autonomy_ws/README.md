# CAREC Autonomy Workspace

This directory will contain the ROS 2 simulation-first autonomy stack. It currently records the workspace boundary only; no ROS package is considered implemented yet.

## Current gate

Package scaffolding is intentionally blocked until [issue #17](https://github.com/vinodkumar1947/CAREC-Project/issues/17) approves the stable ROS interfaces and safety boundary. [Issue #20](https://github.com/vinodkumar1947/CAREC-Project/issues/20) then creates the workspace packages and CI build.

Planned responsibilities are documented in the [autonomy architecture](../docs/architecture/AUTONOMY_ARCHITECTURE.md).

## Contributor environment

Open the repository in its development container and run:

```bash
./scripts/bootstrap.sh --check
```

The baseline environment provides ROS 2 Jazzy, Gazebo integration, Nav2, SLAM Toolbox, C++ tools, and Python test tools.

## Planned package boundaries

```text
carec_interfaces      stable messages, services, actions, and reason codes
carec_description     wheelchair geometry, footprint, and drive profiles
carec_simulation      Gazebo adapters, sensors, worlds, and actors
carec_perception      sensor processing and uncertainty
carec_localization    pose estimation and health
carec_navigation      planning, control, and recovery
carec_shared_control  user-intent arbitration
carec_safety          command limits, watchdogs, collision monitoring, stop
carec_evaluation      scenarios, metrics, and evidence
carec_bringup         reproducible launch configurations
```

Dependencies must flow through reviewed interfaces. Navigation may propose motion; only `carec_safety` may publish to a simulator or future owner-controlled platform adapter.

## Hardware requirement

None. A physical wheelchair, embedded board, sensor, game controller, and GPU are not required for normal autonomy contributions.
