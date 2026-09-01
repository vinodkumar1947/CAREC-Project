# ADR-0009 — Stable CAREC ROS Interfaces

Status: Proposed
Date: 2026-09-01

## Context

CAREC needs stable ROS 2 interface boundaries before the autonomy workspace, simulator, safety supervisor, navigation stack, evaluation tooling, and future wheelchair hardware adapters are implemented.

The project must preserve a hard safety boundary: user-input and autonomy components may propose motion, but only the independent safety supervisor may publish motion that a simulator or future platform adapter is allowed to consume. Simulation and future hardware must use the same safe-motion contract so autonomy code cannot gain a second path around safety supervision.

This ADR defines semantic ownership, message direction, units, frames, timestamps, freshness rules, and health-state conventions. It does not certify any physical wheelchair interface and does not define manufacturer-specific motor protocols.

## Decision

### 1. Interface ownership and command flow

CAREC will use the following logical motion path:

`User Input / Autonomy -> User Intent or Proposed Velocity -> Safety Supervisor -> Safety-Approved Velocity -> Simulator or Platform Adapter`

Rules:

- User-input nodes own user-intent publication.
- Navigation/autonomy nodes may publish proposed velocity but never platform commands.
- The safety supervisor is the only owner of safety-approved velocity.
- Simulation and future hardware/platform adapters subscribe only to safety-approved velocity for commanded motion.
- No planner, teleoperation node, AI component, remote client, simulator helper, or hardware adapter may bypass the safety supervisor.
- User stop intent and emergency-stop state take priority over autonomous motion.
- Remote or cloud-originated requests are treated as intent/proposals and never as trusted platform commands.

### 2. Required logical interfaces

Initial package implementation may use standard ROS messages where semantics are sufficient and CAREC-specific messages where explicit state is required. Exact package/type names may be refined during Issue #20 without changing the contracts below.

#### 2.1 User intent

Purpose: represent operator-requested motion or stop intent before safety filtering.

Required semantics:

- linear velocity intent in meters per second (`m/s`)
- angular velocity intent in radians per second (`rad/s`)
- explicit stop intent
- source identity
- source timestamp
- freshness/validity information

The interface must distinguish an intentional zero-motion command from missing or stale input.

#### 2.2 Proposed velocity

Purpose: carry a motion proposal from teleoperation arbitration, shared control, or autonomous navigation to the safety supervisor.

Required semantics:

- planar linear velocity in `m/s`
- yaw angular velocity in `rad/s`
- source timestamp
- source identity or mode

A proposed velocity is never considered safe merely because it came from Nav2 or another trusted local process.

#### 2.3 Safety-approved velocity

Purpose: the sole commanded-motion interface accepted by the simulator and future platform adapters.

Required semantics:

- bounded planar linear velocity in `m/s`
- bounded yaw angular velocity in `rad/s`
- timestamp corresponding to the safety decision
- freshness deadline enforced by the consumer and/or safety layer

If no fresh valid command is available, the required safe command is zero motion.

#### 2.4 Emergency stop and reset

Emergency stop and reset must be represented explicitly rather than inferred from velocity alone.

Required states:

- emergency stop requested/active
- reset requested
- reset accepted/rejected

Reset must not automatically restore motion. After reset, a new fresh valid motion request is required.

A future physical emergency-stop implementation may include electrical or manufacturer-specific mechanisms outside ROS. ROS state must never be described as a substitute for a required physical emergency stop.

#### 2.5 Sensor health

Every safety-relevant sensor pipeline must expose health independently from its sensor data stream.

Minimum health states:

- `HEALTHY`
- `STALE`
- `UNAVAILABLE`
- `INVALID`

Health reporting should include the observed timestamp and, where useful, a reason/detail field. Sensor consumers must not infer `HEALTHY` solely from node/process existence.

#### 2.6 Localization health

Localization must expose a health state distinct from pose data.

Minimum states:

- `HEALTHY`
- `STALE`
- `UNAVAILABLE`
- `INVALID`

Optional implementation details may include confidence/covariance metrics, but those metrics do not replace the explicit health state.

#### 2.7 Safety intervention reason

Whenever the safety supervisor changes or rejects proposed motion, it must publish a machine-readable intervention reason.

Initial reason categories should support at least:

- none
- emergency stop
- explicit user stop
- stale command
- invalid command
- speed limit
- acceleration/jerk limit
- obstacle stop
- obstacle slowdown
- required sensor unhealthy
- localization unhealthy
- internal safety fault

The implementation may extend this enumeration while preserving backward-compatible meanings.

#### 2.8 Platform profile identity

The system must expose the active platform profile used for parameters such as footprint, drive configuration, motion limits, and simulator/platform adapter selection.

A profile identity must not imply certification for a physical wheelchair model. Physical compatibility requires separate validation and approval.

### 3. Coordinate frames

CAREC will follow standard ROS mobile-robot frame conventions:

- `map` — global map frame when mapping/localization is active
- `odom` — locally continuous odometry frame
- `base_link` — primary wheelchair body frame
- sensor frames — fixed child frames of the modeled platform as appropriate

The expected transform chain is:

`map -> odom -> base_link -> sensor frames`

Motion commands are planar commands interpreted relative to the robot body convention used by ROS mobile bases:

- positive linear X: forward
- positive angular Z: counter-clockwise yaw

Frame IDs, transform ownership, and static sensor transforms must be deterministic and documented in the description/bringup packages.

### 4. Time and freshness

- ROS timestamps use the active ROS clock.
- Simulation must support `/clock` and simulated time.
- Producers timestamp data or commands as close as practical to creation/acquisition.
- Consumers evaluate freshness using explicit configurable timeout values.
- Timeout values must not be hidden constants in application code.
- Clock discontinuity, missing timestamps, NaN/Inf values, and timestamps outside accepted bounds must result in an explicit invalid/stale state rather than silently accepting motion.

Specific timeout values will be configuration-controlled and validated by Issues #25 and #27; this ADR defines the requirement, not physical stopping-time certification.

### 5. Healthy, stale, unavailable, and invalid are distinct

For CAREC interfaces:

- `HEALTHY`: information is present, timely, and passes required validation.
- `STALE`: previously available information exists but exceeds its freshness deadline.
- `UNAVAILABLE`: required information is not currently available or has not been established.
- `INVALID`: information is present but malformed, non-finite, out of allowed bounds, frame-inconsistent, or otherwise fails validation.

Components must not collapse these into a single Boolean when the distinction affects diagnostics, testing, or safe behavior.

### 6. Default safe behavior

At the safety-approved motion boundary, any condition that prevents the supervisor from establishing a fresh valid safe command results in commanded zero motion.

Examples include:

- stale or missing proposed velocity
- active emergency stop
- invalid numeric command
- required safety sensor becoming stale/unavailable/invalid
- internal safety-supervisor fault

Whether localization loss requires immediate zero motion or another bounded safe behavior may depend on the active operating mode and must be explicitly configured and tested. No component may silently continue unrestricted autonomous motion after localization becomes invalid.

### 7. Versioning

CAREC-specific ROS interfaces will be treated as public project contracts once accepted.

- Breaking semantic changes require an ADR update or replacement ADR.
- New optional fields/states must preserve existing meanings where feasible.
- Topic names are configuration/integration details; semantic ownership and direction defined here are architectural constraints.

## Alternatives Considered

### Direct planner or teleoperation output to the platform adapter

Rejected because multiple command sources could bypass safety supervision and create inconsistent timeout, limit, and emergency-stop behavior.

### Separate interfaces for simulation and physical hardware

Rejected as the primary architecture because behavior proven in simulation could bypass or differ from the physical safe-motion boundary. Platform-specific adapters may differ internally but must consume the same safety-approved command semantics.

### Single generic velocity topic shared by all command sources

Rejected because it obscures ownership and cannot distinguish intent, proposal, and safety-approved motion.

### Boolean-only health reporting

Rejected because stale, unavailable, and invalid conditions require different diagnostics and deterministic fault-injection tests.

## Advantages

- Establishes one enforceable motion path through safety supervision.
- Allows simulator and future hardware adapters to share the same safe-motion boundary.
- Makes stale, unavailable, and invalid states testable.
- Gives Issue #20 stable package/interface boundaries.
- Supports deterministic safety and fault-injection testing.
- Keeps AI, navigation, remote-control, and user-input components outside the trusted final-motion boundary.

## Disadvantages

- Adds explicit messages/state and integration work compared with directly publishing `cmd_vel`.
- Requires arbitration and health-reporting components to respect stricter contracts.
- Some exact ROS message definitions remain to be implemented after this semantic ADR is accepted.
- Freshness and safety limits require careful configuration and validation for each future physical platform.

## Safety Impact

This is an S3 architectural decision because interface ownership affects unintended motion and emergency stopping.

The critical invariant is:

> Only the independent safety supervisor may produce the command accepted by the simulator or future physical platform adapter.

ROS interfaces and simulation evidence are not physical-wheelchair safety certification. No physical wheelchair shall be connected or controlled based solely on acceptance of this ADR. Manufacturer-specific electrical/protocol interfaces, safe states, watchdog behavior, braking behavior, emergency-stop implementation, and bench evidence require separate review and approval.

## Consequences

- Issue #20 may scaffold packages for interfaces, description, simulation, safety, evaluation, and bringup only after this ADR is reviewed/accepted as required by Issue #17.
- Issue #24 teleoperation must publish user intent rather than drive the simulator directly.
- Issue #25 safety supervision must be the only publisher/owner of safety-approved platform motion.
- Issue #26 collision monitoring must act independently of the planner and feed/enforce the safety boundary.
- Issue #27 fault injection must test stale, unavailable, invalid, and process-failure states.
- Issue #28 localization must publish explicit health in addition to pose/localization data.
- Issue #30 shared control must preserve user stop priority and safety-supervisor ownership.
- Future platform adapters governed by ADR-0004 must reject raw planner/autonomy output.

## Evidence and Validation

Before this ADR is changed from `Proposed` to `Accepted`:

- Review the interface ownership diagram against ADR-0004 and ADR-0008.
- Obtain at least two reviewer approvals as required by GitHub Issue #17.
- Confirm every required Issue #17 interface is represented.
- Confirm units, frames, timestamps, and freshness semantics are unambiguous.
- Confirm no simulator or future hardware adapter path can consume raw planner output by design.

Implementation evidence will later include build/test results from Issues #20, #24, #25, #27, and the simulation release evidence from Issue #32.

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
