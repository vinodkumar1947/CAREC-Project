# ADR-0002 — ROS 2 Distribution

Status: Proposed
Date: 2026-08-23

## Context
The project needs a supported ROS 2 baseline compatible with simulator, Nav2, CI, and contributor systems.

## Decision
TBD. ROS 2 Jazzy on Ubuntu 24.04 is a candidate.

## Alternatives Considered
Current supported ROS 2 LTS/non-LTS distributions and containerized multi-version support.

## Advantages
A single baseline improves reproducibility and documentation.

## Disadvantages
It can exclude platforms and couple CAREC to one support window.

## Safety Impact
Middleware lifecycle, QoS, clocks, and dependency support affect fault behavior.

## Consequences
Document compatibility evidence, upgrade policy, and pinned environment before acceptance.
