# CAREC Hardware Boundary

> CAREC Sim requires no physical hardware. This directory records the single owner-controlled prototype and possible future integration requirements. It is not a shopping guide or deployment authorization.

## Current prototype

The repository retains experimental documentation for one SenseCAP Watcher W1-A obstacle-warning device and a removable camera-mount concept. This hardware does not provide autonomous navigation or universal wheelchair compatibility.

No contributor is expected to buy, borrow, receive, mount, or flash:

- a wheelchair;
- a camera or LiDAR;
- an embedded controller;
- a motor interface;
- a GPU; or
- prototype mounting hardware.

## Compatibility rule

There is no universal powered-wheelchair electrical or mechanical interface. CAREC separates:

1. a platform-agnostic autonomy core;
2. configurable simulation profiles; and
3. individually reviewed and validated physical adapters.

A simulation profile does not certify a physical wheelchair. Manual wheelchairs require a separate powered drive system.

## Owner-only physical work

Physical experiments, when authorized later, must progress through:

1. interface review and hazard analysis;
2. simulation regression evidence;
3. hardware-in-the-loop bench testing;
4. wheels-raised testing;
5. tethered, unoccupied, low-speed testing;
6. supervised evaluation under an approved protocol.

Community code must never communicate directly with wheelchair motors. The future `hardware_bridge/` remains disabled by default and accepts commands only from the independent safety supervisor.

## Purchasing policy

Do not purchase additional hardware during the contributor-foundation or digital-wheelchair milestones. Hardware selection begins only after simulation has measured required range, field of view, latency, compute, power, braking behavior, and interface needs.

The legacy [bill of materials](BOM.md), [hardware status](../docs/specifications/HARDWARE_STATUS_UPDATE.md), and [mounting guide](../docs/guides/mounting_guide.md) are retained for provenance; their prices and deployment claims are not current commitments.

## Datasheets

See [datasheets/README.md](datasheets/README.md) for legacy prototype references. Future components must be added only with a documented requirement and architecture decision.
