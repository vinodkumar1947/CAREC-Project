# Initial Proposed Backlog

Status: Proposed
Owner: Project Maintainer
Last Updated: 2026-08-23
Related Issues: These are proposals; GitHub issues are not yet created
Related ADRs: ADR-0001 through ADR-0008

Priorities are proposals. `No` hardware items should be favored in M0. “GFI” means suitable for `difficulty:good-first-issue` after a mentor is assigned.

| ID | Title | Workstream | Priority | Difficulty | Hardware | Description | Deliverable | Acceptance criteria | Dependencies |
|---|---|---|---|---|---|---|---|---|---|
| CARE-001 | Compare simulation platforms | Architecture/Simulation | P0 | Intermediate | No | Score candidates for M0 | ADR evidence matrix + headless PoC | Criteria cover ROS 2, CI, sensors, licensing, resources; recommendation reviewed | None |
| CARE-002 | Select ROS 2 baseline | Architecture/ROS2 | P0 | Intermediate | No | Compare supported distributions | ADR update + compatibility table | Simulator/Nav2/Ubuntu/CI evidence documented | CARE-001 |
| CARE-003 | Define M0 interface inventory | Architecture | P0 | Intermediate | No | Identify component contracts | Interface catalog | Producers, consumers, units, frames, rates, timeouts, safe defaults listed | CARE-002 |
| CARE-004 | Create ROS 2 workspace skeleton | ROS2 | P0 | Beginner | No | Add buildable package layout | Workspace and README | Clean container build/test succeeds | CARE-002 |
| CARE-005 | Add one-command simulation launch | Simulation | P0 | Intermediate | No | Launch model, world, sensors, RViz optional | Launch command/script | Headless launch exits cleanly and docs reproduce it | CARE-001, CARE-004 |
| CARE-006 | Model wheelchair base geometry | Simulation | P0 | Beginner | No | Create parameterized base/wheels/casters | URDF/SDF model | Spawns with valid inertial/collision elements and documented TBD values | CARE-001 |
| CARE-007 | Implement differential drive | Simulation | P0 | Intermediate | No | Connect bounded velocity to wheel motion | Drive plugin/config + test | Straight and turn scenarios meet declared tolerance | CARE-006 |
| CARE-008 | Add keyboard teleoperation | Simulation | P1 | GFI | No | Provide manual simulated input | Teleop config/launch/docs | User can command and stop; timeout is documented | CARE-005, CARE-007 |
| CARE-009 | Publish simulated odometry and IMU | Simulation | P0 | Beginner | No | Add pose/motion observations | Sensor config + topic test | Rates, frames, units, and configurable noise verified | CARE-006, CARE-007 |
| CARE-010 | Publish simulated range sensor | Simulation | P0 | GFI | No | Add generic range measurements | Sensor config + automated test | Configurable rate/noise; obstacle range test passes | CARE-006 |
| CARE-011 | Create baseline indoor world | Simulation | P1 | GFI | No | Add corridor, doorway, furniture | Licensed world asset + map | Dimensions/provenance documented; model spawns repeatably | CARE-001 |
| CARE-012 | Define M0 scenario schema | Simulation/Testing | P0 | Intermediate | No | Represent setup, commands, faults, metrics | JSON/YAML schema + examples | Validation catches missing fields; two scenarios included | CARE-003 |
| CARE-013 | Document initial TF tree | ROS2 | P0 | GFI | No | Specify map/odom/base/sensor frames | TF diagram and contract | Authority, units, frame rules, validation command included | CARE-003 |
| CARE-014 | Define diagnostics conventions | ROS2 | P1 | Beginner | No | Standardize health/fault output | Convention + example node | Severity, source, timestamp, reason, recovery fields tested | CARE-003 |
| CARE-015 | Add launch smoke test | ROS2/Testing | P0 | Beginner | No | Detect startup and topic failures | Headless CI test | Required nodes/topics/TF appear before timeout | CARE-004, CARE-005 |
| CARE-016 | Build mapping baseline | Navigation | P1 | Intermediate | No | Map the baseline world | Launch/config/map/evidence | Repeatable map produced with documented procedure | CARE-005, CARE-009, CARE-011 |
| CARE-017 | Build localization baseline | Navigation | P1 | Intermediate | No | Localize in saved map | Config + scenario report | Pose converges and loss is detected by defined metric | CARE-016 |
| CARE-018 | Configure supervised goal navigation | Navigation | P1 | Advanced | No | Navigate to fixed indoor goals | Config + scenario evidence | Completes baseline routes collision-free or safely fails | CARE-017 |
| CARE-019 | Add blocked-route recovery scenario | Navigation | P1 | Intermediate | No | Test dynamic blockage and cancellation | Scenario + metrics | Replans or stops within timeout; no collision | CARE-018 |
| CARE-020 | Create doorway image dataset spec | Vision | P2 | GFI | No | Define labels and metadata | Dataset card/schema | Licensing, privacy, splits, edge cases, exclusions documented | None |
| CARE-021 | Implement doorway baseline detector | Vision | P2 | Intermediate | No | Establish non-safety baseline | Code, tests, evaluation | Reproducible metrics on versioned sample data | CARE-020 |
| CARE-022 | Build synthetic people-crossing data | Vision/Simulation | P2 | Intermediate | No | Generate varied crossing sequences | Assets + provenance + examples | Seeded generation and scenario coverage documented | CARE-011 |
| CARE-023 | Create AI experiment template | AI/ML | P2 | GFI | No | Standardize experiments/model cards | Template + example | Captures data, model, seed, metrics, latency, limits | None |
| CARE-024 | Benchmark perception uncertainty | AI/ML | P2 | Advanced | No | Measure confidence under degradation | Notebook/script + report | Lighting/occlusion/noise slices and failures reported | CARE-021, CARE-023 |
| CARE-025 | Define synthetic sensor message fixtures | Sensor Fusion | P1 | GFI | No | Provide timestamped multi-sensor data | Fixture set + schema | Valid, stale, missing, contradictory cases included | CARE-003 |
| CARE-026 | Prototype freshness/consistency monitor | Sensor Fusion | P1 | Intermediate | No | Detect stale/inconsistent sources | Library/node + unit tests | Each fixture yields deterministic health state | CARE-025 |
| CARE-027 | Specify MCU health protocol | Firmware | P1 | Intermediate | No | Define heartbeats, faults, reset | Protocol spec + mock | Timeout, version, CRC/auth, safe default covered | CARE-003, ADR-0004 |
| CARE-028 | Expand portable watchdog tests | Firmware/Testing | P1 | GFI | No | Add boundary/fault cases to current logic | Host tests | Timeout, rollover/time assumptions, latch/reset covered | None |
| CARE-029 | Document wheelchair interface constraints | Hardware | P1 | Advanced | Yes | Gather model-specific authoritative data | Reviewed interface dossier | Sources, electrical/mechanical limits, unknowns, no unsafe instructions | Maintainer access |
| CARE-030 | Design bench interface simulator | Hardware/Firmware | P2 | Advanced | Yes | Emulate controller feedback safely | Fixture design + test plan | Isolation, current limits, faults, emergency disconnect reviewed | CARE-027, CARE-029 |
| CARE-031 | Create accessible mobile wireframes | Mobile | P2 | GFI | No | Design status/mode/stop flows | Wireframes + accessibility notes | Keyboard/screen-reader/color-independent review checklist passes | None |
| CARE-032 | Implement mock mobile state client | Mobile | P2 | Intermediate | No | Render simulated state only | Prototype + tests | Offline, stale data, fault, stop confirmation states shown | CARE-031, CARE-003 |
| CARE-033 | Define privacy-preserving telemetry schema | Cloud | P2 | Beginner | No | Specify optional minimal telemetry | Schema + threat/privacy notes | No identifiers by default; consent/retention/version fields defined | CARE-003 |
| CARE-034 | Build local mock telemetry service | Cloud | P3 | Intermediate | No | Test backend without internet | Service + contract tests | Accepts valid schema; rejects unauthorized/invalid data | CARE-033 |
| CARE-035 | Approve safety severity/probability scales | Safety | P0 | Advanced | No | Define risk-rating method | Reviewed scales and examples | Ambiguity/unknown handling and acceptance authority explicit | None |
| CARE-036 | Review preliminary hazard register | Safety | P0 | Intermediate | No | Complete event sequences and controls | Updated register | Named hazards have owners, controls, verification, status | CARE-035 |
| CARE-037 | Create safety fault-injection catalog | Safety/Testing | P0 | Intermediate | No | Map failures to injectable scenarios | Catalog | Sensor, compute, comms, localization, AI, power proxies covered | CARE-012, CARE-036 |
| CARE-038 | Specify e-stop state machine | Safety | P0 | Advanced | No | Define priority, latch, reset | State diagram/spec/tests | Reset never commands motion; all transitions deterministic | ADR-0004, CARE-036 |
| CARE-039 | Create AI safety review checklist | Safety/AI | P1 | GFI | No | Standardize model integration review | Checklist | Provenance, slices, uncertainty, fallback, monitoring included | CARE-023, CARE-036 |
| CARE-040 | Add Markdown link checker | Testing/Docs | P1 | GFI | No | Catch broken internal links in CI | Script/workflow + docs | Runs locally/CI and ignores documented external exceptions | None |
| CARE-041 | Add ROS interface contract tests | Testing/ROS2 | P1 | Intermediate | No | Verify messages/frames/timeouts | Test suite | Deliberate schema/frame/freshness faults fail tests | CARE-003, CARE-004 |
| CARE-042 | Generate scenario evidence report | Testing | P1 | Intermediate | No | Summarize machine-readable runs | Report tool + fixture | Includes config, revision, metrics, result, limitations | CARE-012 |
| CARE-043 | Validate clean contributor setup | Documentation/DX | P0 | GFI | No | Test setup from clean environment | Repro log + fixes | Documented commands succeed without hardware | CARE-004, CARE-005 |
| CARE-044 | Add coding/formatting baseline | Documentation/DX | P1 | Beginner | No | Propose Python/C++/Markdown tools | Config + CI + guide | Commands documented; existing code impact reviewed | CARE-004 |
| CARE-045 | Annotate ten related projects/papers | Research | P2 | GFI | No | Build sourced research baseline | Ten annotated entries | Each includes date, license/citation, evidence, limits, relevance | None |
| CARE-046 | Add legacy document banners | Documentation | P1 | GFI | No | Clarify historical versus active docs | Reviewed banner updates | All listed legacy files link to current docs and retain content | Migration report |

Before publishing any row as a GitHub issue, assign a mentor/owner, select labels and safety class, expand testing commands and relevant files, and confirm dependencies. Suggested initial good-first issues: CARE-008, 010, 011, 013, 020, 023, 025, 028, 031, 039, 040, 043, 045, and 046.
