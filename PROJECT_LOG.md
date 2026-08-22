# CAREC Project Log

## August 22, 2026 — Simulation-first reset

### Direction established

- Reframed CAREC as an open-source, simulation-first assistive mobility research platform.
- Defined shared control as the first major capability objective.
- Made no contributor hardware the default.
- Isolated future physical integration behind owner-controlled adapters.
- Replaced the claim of universal physical compatibility with a platform-agnostic core and validated model-specific adapters.

### Existing evidence retained

- SenseCAP Watcher firmware prototype and related tests remain available as research inputs.
- Existing firmware does not control wheelchair motors.
- Existing distance estimation is monocular and requires physical calibration.
- The autonomy workspace, digital wheelchair, and safety kernel are not yet implemented.

### Immediate next actions

1. Configure GitHub branch protection and limited team access.
2. Create the GitHub Project from the published roadmap.
3. Have all ten contributors complete the practice pull-request exercise.
4. Agree on simulation interfaces and package ownership.
5. Implement the reproducible ROS 2/Gazebo development environment.
6. Resolve firmware failure-state contradictions before reusing its decision logic.

Historical documents may describe earlier plans. Current direction is governed by the root README, ROADMAP, CONTRIBUTING guide, and `docs/vision/` documents.
