# ADR-0003 — Initial Sensor Suite

Status: Proposed
Date: 2026-08-23

## Context
Simulation and future hardware need complementary pose, motion, obstacle, and drop information.

## Decision
TBD after hazard coverage and failure analysis. M0 will simulate odometry, IMU, and at least one generic range source without claiming physical selection.

## Alternatives Considered
Camera/depth, 2D/3D LiDAR, ultrasonic, ToF, encoders, IMU, and dedicated drop sensors.

## Advantages
A small initial suite accelerates interface and fault-model work.

## Disadvantages
Insufficient diversity or blind-zone coverage could create unsafe assumptions.

## Safety Impact
Selection affects obstacle/drop detection, degraded modes, privacy, and single-point failures.

## Consequences
Separate simulation message contracts from later vendor/model selection.
