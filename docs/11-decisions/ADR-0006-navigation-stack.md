# ADR-0006 — Navigation Stack

Status: Proposed
Date: 2026-08-23

## Context
Indoor mapping, localization, planning, control, and recovery need a maintainable baseline.

## Decision
TBD. Nav2 plus SLAM Toolbox is the initial candidate for evaluation.

## Alternatives Considered
Nav2 configurations, alternative SLAM/localization packages, and custom wheelchair-aware planners/controllers.

## Advantages
Mature ROS 2 integration and plugin architecture may accelerate M0/M1 work.

## Disadvantages
Defaults may not reflect wheelchair comfort, footprint, doors, or safety constraints.

## Safety Impact
Planner/controller failure must remain subordinate to independent safety controls.

## Consequences
Benchmark doorway, corridor, blockage, recovery, and localization-loss scenarios before acceptance.
