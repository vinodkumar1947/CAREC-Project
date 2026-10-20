# ADR-0009 — Stable CAREC ROS Interfaces

Status: Accepted
Date: 2026-09-01
Accepted: 2026-09-01

## Context

CAREC needs stable ROS 2 interface boundaries before the autonomy workspace, simulator, safety supervisor, navigation stack, evaluation tooling, and any future platform adapter are implemented.

The current CAREC engineering scope is simulation-first ROS 2 autonomy. User-input and autonomy components may propose motion, but only the independent safety supervisor may publish motion that the simulator is allowed to consume. A future physical platform adapter, if introduced under a separately reviewed hardware phase, must use the same safe-motion contract.

This ADR defines semantic ownership, message direction, units, coordinate frames, timestamps, freshness rules, health-state conventions, and safe defaults. It does not certify or authorize physical wheelchair control.

## Decision

### 1. Command ownership and flow

The required logical command path is:

`User Input / Autonomy -> User Intent or Proposed Velocity -> Safety Supervisor -> Safety-Approved Velocity -> Simulator`

For future physical integration, only a separately approved platform adapter may replace the simulator at the final boundary.

Rules:

- User-input nodes own user-intent publication.
- Navigation, shared-control, and autonomy nodes may publish proposed velocity but never final platform commands.
- The independent safety supervisor is the only owner of safety-approved velocity.
- The simulator and any future approved platform adapter subscribe only to safety-approved velocity for commanded motion.
- No planner, teleoperation node, AI component, remote client, simulator helper, or platform adapter may bypass the safety supervisor.
- Explicit user stop and emergency-stop state take priority over autonomous motion.

### 2. Required logical interfaces

The ROS 2 implementation must provide these semantic interfaces. Exact package, topic, service, and message names may be refined during Issue #20 without changing these contracts.

#### User intent

Must represent:

- linear velocity intent in meters per second (`m/s`)
- angular velocity intent in radians per second (`rad/s`)
- explicit stop intent
- source identity
- source timestamp
- freshness or validity

An intentional zero-motion command must be distinguishable from missing or stale input.

#### Proposed velocity

Carries motion proposed by teleoperation arbitration, shared control, or autonomous navigation to the safety supervisor.

Must include planar linear velocity in `m/s`, yaw angular velocity in `rad/s`, source timestamp, and source identity or operating mode.

A proposed velocity is never trusted as safe solely because it came from Nav2 or another local process.

#### Safety-approved velocity

This is the sole commanded-motion interface accepted by the simulator and any future approved platform adapter.

It must contain bounded planar linear velocity in `m/s`, bounded yaw angular velocity in `rad/s`, the safety-decision timestamp, and a configurable freshness deadline. If a fresh valid command cannot be established, the safe command is zero motion.

#### Emergency stop and reset

Emergency stop and reset are explicit state transitions, not values inferred from velocity.

The system must represent emergency-stop requested/active state, reset requested state, and reset accepted/rejected state. Reset must never automatically resume motion; a new fresh valid motion request is required after reset.

ROS emergency-stop state is not a substitute for any future physical emergency-stop mechanism.

#### Sensor health

Every safety-relevant sensor pipeline must expose health independently from its sensor data stream using at least:

- `HEALTHY`
- `STALE`
- `UNAVAILABLE`
- `INVALID`

A process being alive is not evidence that its data is healthy.

#### Localization health

Localization health is distinct from pose data and uses at least the same four states: `HEALTHY`, `STALE`, `UNAVAILABLE`, and `INVALID`.

Confidence or covariance may supplement but not replace explicit health state.

#### Safety intervention reason

Whenever the safety supervisor changes or rejects a proposed command, it publishes a machine-readable reason. Initial categories include none, emergency stop, user stop, stale command, invalid command, speed limit, acceleration or jerk limit, obstacle stop, obstacle slowdown, required sensor unhealthy, localization unhealthy, and internal safety fault.

#### Platform profile identity

The system exposes the active profile governing parameters such as footprint, drive model, motion limits, and simulator selection. A profile identity does not imply physical-wheelchair compatibility or certification.

### 3. Coordinate frames

CAREC follows standard ROS mobile-robot conventions:

- `map` — global map frame when localization or mapping is active
- `odom` — locally continuous odometry frame
- `base_link` — primary wheelchair body frame
- sensor frames — deterministic child frames of the modeled platform

Expected transform chain:

`map -> odom -> base_link -> sensor frames`

Positive linear X is forward. Positive angular Z is counter-clockwise yaw.

### 4. Time, freshness, and validation

- ROS timestamps use the active ROS clock.
- Simulation supports `/clock` and simulated time.
- Producers timestamp data and commands close to creation or acquisition.
- Consumers evaluate freshness using explicit configurable timeout values.
- Safety-relevant timeout values must not be hidden constants.
- Missing timestamps, clock discontinuities, non-finite numeric values, out-of-range values, and frame inconsistencies become explicit stale or invalid states rather than being silently accepted.

Specific timeout values remain configuration-controlled and will be validated by safety and fault-injection work.

### 5. Health-state semantics

- `HEALTHY`: present, timely, and validated.
- `STALE`: previously available but older than the freshness deadline.
- `UNAVAILABLE`: required information is absent or has not been established.
- `INVALID`: information is present but malformed, non-finite, out of bounds, frame-inconsistent, or otherwise fails validation.

These states must remain distinguishable wherever diagnostics, testing, or safe behavior depend on the difference.

### 6. Default safe behavior

At the safety-approved motion boundary, inability to establish a fresh valid safe command results in commanded zero motion.

Examples include stale or missing proposed velocity, active emergency stop, invalid numeric command, required safety sensor becoming unhealthy, or internal safety-supervisor fault.

Localization loss may use a mode-specific bounded response only when explicitly configured and tested. Unrestricted autonomous motion must never continue silently after localization becomes invalid.

### 7. Versioning

Once accepted, CAREC ROS interfaces are public project contracts.

- Breaking semantic changes require an ADR update or replacement ADR.
- New optional fields or states must preserve existing meanings where feasible.
- Topic names are integration details; semantic ownership and direction are architectural constraints.

## Alternatives Considered

### Direct planner or teleoperation output to the simulator/platform adapter

Rejected because it would permit safety bypass and inconsistent timeout, limit, and emergency-stop behavior.

### Separate command semantics for simulation and future hardware

Rejected because simulation evidence must exercise the same final safe-motion boundary intended for later integration.

### One generic velocity topic for all command sources

Rejected because it obscures ownership and cannot distinguish intent, proposal, and safety-approved motion.

### Boolean-only health reporting

Rejected because stale, unavailable, and invalid conditions require different diagnostics and deterministic tests.

## Safety Impact

This is a high-impact architectural decision because command ownership affects unintended motion and stopping behavior.

The invariant is:

> Only the independent safety supervisor may produce the motion command accepted by the simulator or a future approved physical platform adapter.

Acceptance of this ADR is not physical-wheelchair safety certification and does not authorize physical wheelchair control.

## Consequences

- Issue #20 can now scaffold the ROS 2 packages for interfaces, description, simulation, safety, evaluation, and bringup.
- Teleoperation publishes user intent rather than commanding the simulator directly.
- Safety supervision is the only owner of final safety-approved motion.
- Collision monitoring acts independently of the planner and feeds or enforces the safety boundary.
- Fault injection tests stale, unavailable, invalid, numeric-error, and process-failure states.
- Localization publishes explicit health as well as localization data.
- Shared control preserves explicit user-stop priority and safety-supervisor ownership.

## Acceptance Record

The ADR was technically reviewed against Issue #17 and the simulation-first CAREC architecture on 2026-09-01. The technical review found no interface-contract gaps.

Issue #17 originally requested two independent reviewer approvals. On 2026-09-01, the project owner explicitly removed the repository branch-rule gate and instructed the project to proceed. PR #45 was then merged by owner decision. This acceptance record makes that governance override explicit rather than representing independent approvals that did not occur.

Future changes to this safety boundary should return to independent review before acceptance whenever reviewers are available.

## Related Issues

- #17 — Define stable CAREC ROS interfaces in an ADR
- #20 — Scaffold the ROS 2 autonomy workspace
- #24 — Add keyboard teleoperation and baseline telemetry
- #25 — Implement command timeout and motion-limit supervisor
- #26 — Configure footprint-aware collision monitoring
- #27 — Build deterministic fault-injection scenario framework
- #28 — Establish mapping and localization baseline
- #30 — Combine user intent with safety-filtered motion
- #32 — Produce CAREC Sim 0.1 evidence report

Related decisions:

- ADR-0004 — Motor-Control Interface
- ADR-0008 — Communication Architecture
