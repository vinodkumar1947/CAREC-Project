# CAREC Safety Plan

## Scope

This plan governs simulation and unoccupied engineering prototypes. It does not
authorize use with an occupant.

## Responsibilities

- Product/release owner: approves intended use, residual risk, releases and any
  physical integration.
- Change author: identifies affected requirements, hazards and tests.
- Reviewer: verifies evidence independently of the author.
- Future clinical/human-factors advisers: review user-facing risks before human
  evaluation.

## Risk process

1. Identify hazard, foreseeable event sequence, hazardous situation and harm.
2. Estimate initial severity and probability using documented definitions.
3. Prefer inherent design controls, then protective measures, then information.
4. Link each control to a requirement and verification test.
5. Record evidence and re-estimate residual risk.
6. Owner explicitly accepts or rejects residual risk; supervision alone is not
   automatic acceptance.

## Safe state

For the simulation and command interface, the default safe state is a fresh
zero-linear and zero-angular command plus a diagnostic reason. A real wheelchair
may require a different manufacturer-approved stop mechanism; that must be
defined per adapter before connection.

## Release gate

A release requires passing CI, a complete traceability check, resolved critical
hazards, recorded limitations, reproducible scenario results, an SBOM, and owner
approval. Physical enablement requires additional bench evidence.
