# ADR-0004 — Motor-Control Interface

Status: Proposed
Date: 2026-08-23

## Context
Community software needs a safe-motion abstraction while real wheelchair controls are model-specific and safety-critical.

## Decision
TBD. Proposed direction: only an independent safety supervisor emits bounded, fresh commands to a disabled-by-default platform adapter.

## Alternatives Considered
Direct ROS-to-controller commands; separate safety MCU; manufacturer-approved interface; external retrofit controller.

## Advantages
Isolation enables simulation, mocks, review, watchdogs, and wheelchair-specific validation.

## Disadvantages
Additional latency, hardware, interface design, and verification effort.

## Safety Impact
This is an S3 decision affecting unintended motion and emergency stopping.

## Consequences
No physical implementation until electrical, protocol, safe-state, timeout, feedback, and bench evidence are approved.
