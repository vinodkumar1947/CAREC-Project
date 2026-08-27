# ADR-0001 — Simulation Platform

Status: Proposed
Date: 2026-08-23

## Context
M0 requires differential-drive physics, sensors, indoor worlds, headless CI, and ROS 2 integration on ordinary contributor hardware.

## Decision
TBD after a scored comparison and proof of concept. A current Gazebo release is the leading candidate, not an approved commitment.

## Alternatives Considered
Gazebo; Webots; Isaac Sim as an optional high-fidelity backend; lightweight custom/reference simulation.

## Advantages
A standard simulator may reduce integration work and improve ecosystem compatibility.

## Disadvantages
Version coupling, compute cost, nondeterminism, and asset maintenance may burden contributors.

## Safety Impact
Simulation fidelity and fault injection affect evidence quality but cannot prove physical safety.

## Consequences
CARE-001 must define criteria, run a headless experiment, and recommend a reversible M0 choice.
