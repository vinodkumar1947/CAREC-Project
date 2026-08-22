# CAREC Roadmap

**Program start:** August 2026

**Strategy:** simulation first, CPU first, hardware isolated

**First release objective:** CAREC Sim 0.1 — Digital Wheelchair + Safety Kernel

Progress is awarded only when code is merged, automated tests pass, documentation exists, and acceptance evidence is recorded. Issue counts and lines of code are not progress measures.

## Twelve-week foundation program

### Weeks 1–2 — Contribution foundation

- Publish vision, non-goals, architecture, governance, and safety boundaries.
- Protect `main`; require pull requests and passing checks.
- Train all contributors with a documentation-only practice pull request.
- Provide issue and PR templates, ownership rules, and definition of done.
- Specify the development container and one-command workflow.
- Resolve contradictions between firmware failure behavior, tests, and documentation.

**Exit evidence:** every contributor completes one reviewed pull request; repository rules and CI are active.

### Weeks 3–5 — Digital wheelchair

- Create ROS 2 workspace and packages.
- Model a generic powered wheelchair with configurable geometry.
- Support differential, mid-wheel, and rear-wheel simulation profiles.
- Add odometry, IMU, camera, depth, and LiDAR simulation interfaces.
- Add keyboard/joystick teleoperation.
- Build accessible apartment and corridor scenarios.
- Record collisions, clearance, commands, and timing.

**Exit evidence:** a clean checkout can launch and manually drive the simulated wheelchair with no special hardware.

### Weeks 6–8 — Independent safety kernel

- Enforce speed, acceleration, and jerk limits.
- Add command timeout and sensor-freshness monitoring.
- Add footprint-aware collision monitoring.
- Implement latched emergency-stop and explicit reset behavior.
- Inject process crash, stale sensor, bad localization, and delayed-command faults.
- Produce a machine-readable safety report.

**Exit evidence:** every defined injected failure reaches a deterministic safe state in simulation.

### Weeks 9–10 — Navigation baseline

- Map a simulated environment.
- Localize and navigate to a selected destination.
- Avoid static obstacles and replan around blockage.
- Validate doorway and corridor behavior.

**Exit evidence:** repeatable supervised point-to-point navigation with zero collision in the baseline scenario set.

### Weeks 11–12 — Shared control and Sim 0.1

- Combine user direction with safety-filtered velocity control.
- Add a moving-person crossing scenario.
- Measure interventions, false stops, clearance, comfort, and latency.
- Run complete regression suite and publish results.
- Tag and document CAREC Sim 0.1.

**Exit evidence:** a user can drive virtually while the independent safety layer rejects unsafe commands.

## Weighted program score

| Deliverable | Weight |
|---|---:|
| Reproducible contributor setup | 10% |
| Digital wheelchair and sensors | 15% |
| Safety kernel | 20% |
| Scenario and metrics framework | 15% |
| Shared control | 15% |
| Supervised navigation | 10% |
| Perception research | 5% |
| Hardware-in-the-loop validation | 5% |
| Controlled physical validation | 5% |

See [docs/status/PROJECT_SCORECARD.md](docs/status/PROJECT_SCORECARD.md) for earned progress and evidence.

## Deferred until after Sim 0.1

- Purchase of new sensors or compute hardware.
- Direct connection to occupied-wheelchair motor controls.
- Outdoor or road navigation claims.
- Unsupervised operation.
- Requirement for paid cloud services or high-end GPUs.
- Claims of compatibility with a physical wheelchair without model-specific validation.
