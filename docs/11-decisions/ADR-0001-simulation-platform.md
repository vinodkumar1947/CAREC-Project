# ADR-0001 — Simulation Platform

Status: Proposed
Date: 2026-08-29

## Context
M0 requires differential-drive physics, sensors, indoor worlds, headless CI, and ROS 2 integration across the supported contributor environments.

## Decision
The project baseline is Ubuntu 24.04 LTS, ROS 2 Jazzy, and Gazebo Harmonic, with Nav2, SLAM Toolbox, RViz2, ros2_control/gz_ros2_control, and ros_gz forming the initial robotics stack. Headless build and test workflows are the portable baseline. Graphical simulation and GPU acceleration remain host-dependent and must be validated on each supported platform.

NVIDIA Isaac Sim is reserved for a later high-fidelity simulation phase and does not replace the baseline contributor environment.

## Alternatives Considered
Gazebo; Webots; Isaac Sim as an optional high-fidelity backend; lightweight custom/reference simulation.

## Advantages
A standard ROS 2-centered simulator stack reduces integration variation, improves reproducibility, and aligns contributor work with the project's navigation and testing architecture.

## Disadvantages
Version coupling, compute demand, nondeterminism, graphics compatibility, and asset maintenance may burden contributors and require platform-specific validation.

## Safety Impact
Simulation fidelity and fault injection affect evidence quality but cannot prove physical safety.

## Consequences
Simulation work should target the documented baseline first. Platform-specific GUI/GPU behavior must be recorded as verified only after smoke testing. Advanced simulator integrations must preserve the same CAREC interfaces, scenarios, safety assertions, and evidence model so simulation backends remain replaceable.
