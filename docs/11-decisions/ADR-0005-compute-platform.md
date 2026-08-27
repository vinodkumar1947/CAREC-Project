# ADR-0005 — Compute Platform

Status: Proposed
Date: 2026-08-23

## Context
Future onboard compute must support robotics workloads within power, thermal, cost, availability, and maintainability constraints.

## Decision
TBD; no board or accelerator is selected for autonomy.

## Alternatives Considered
x86 mini-PC, ARM SBC, GPU/NPU modules, split high-level compute plus safety MCU.

## Advantages
Deferring selection keeps software portable and lets measurements guide cost/performance.

## Disadvantages
Hardware optimization and packaging remain unresolved.

## Safety Impact
Boot, thermal, power, watchdog, update, and compute-failure behavior matter.

## Consequences
Define workload benchmarks and safety architecture before procurement.
