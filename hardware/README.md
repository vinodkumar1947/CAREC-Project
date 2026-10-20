# CAREC Hardware Boundary

> The current CAREC milestone is simulation-first. No physical wheelchair hardware is required for contributor work.

## Current scope

The active project does **not** use ESP-IDF, ESP32, SenseCAP Watcher, or any device-specific embedded prototype.

Current engineering targets a platform-agnostic ROS 2 autonomy stack and simulated wheelchair. Physical integration is deferred until the simulation, safety, navigation, and interface milestones have reproducible evidence.

## Future physical integration rules

CAREC separates:

1. platform-agnostic autonomy and safety logic;
2. configurable simulation profiles; and
3. future individually reviewed physical platform adapters.

A simulation profile does not certify a physical wheelchair. Future hardware adapters must never receive raw planner, AI, or user commands; they may consume only the safety-approved motion interface.

## Hardware selection policy

Do not purchase or standardize sensors, embedded controllers, motor interfaces, or compute hardware during CAREC Sim 0.1 solely to begin development. Hardware selection should follow measured simulation requirements such as sensor range, field of view, latency, compute load, braking assumptions, power, and interface constraints.

Any future hardware selection must be introduced through a reviewed requirement and ADR.

For current work, start with the [simulation documentation](../docs/04-simulation/README.md), [ROS 2 engineering guide](../docs/05-engineering/ros2.md), and [GitHub Issues](https://github.com/vinodkumar1947/CAREC-Project/issues).
