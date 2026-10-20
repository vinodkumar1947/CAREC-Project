# CAREC Tests

The active test suite supports the simulation-first ROS 2 autonomy direction. Legacy embedded-firmware tests have been removed from the current project tree.

## Current structure

```text
tests/
├── integration/                  integration and launch tests as ROS 2 packages arrive
├── unit/                         autonomy and simulation unit tests
└── results/                      generated evidence, mostly gitignored
```

## Run current tests

```bash
python3 -m pytest tests/unit -v
```

As the ROS 2 workspace is scaffolded, CI will add package builds, launch smoke tests, interface contract tests, deterministic scenarios, and machine-readable evaluation output.

## Test principles

- Simulation success is not physical-wheelchair safety evidence.
- Safety behavior must be deterministic and independently testable.
- Fixed seeds should be used where practical.
- Scenario evidence should include collisions, minimum clearance, stopping behavior, intervention reason, latency, and final state.
- Future physical testing requires a separate reviewed safety process.

See [Testing Strategy](../docs/07-testing/testing-strategy.md) and [Simulation Testing](../docs/07-testing/simulation-testing.md).
