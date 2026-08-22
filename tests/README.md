# CAREC Tests

The current tests protect the retained firmware decision logic while the ROS 2 scenario framework is being designed. They are not evidence of physical wheelchair safety.

## Current structure

```text
tests/
├── cpp/
│   └── safety_decision_test.cpp  portable production C++ safety boundary
├── obstacle_test.py              legacy Python behavior and fixture tests
├── conftest.py                   shared Python constants and helpers
├── fixtures/
│   └── sample_detections.json
├── integration/                  reserved for future integration tests
├── unit/                         reserved for additional unit tests
└── results/                      generated output, mostly gitignored
```

## Run the portable C++ safety test

```bash
g++ -std=c++17 -Wall -Wextra -Werror \
  -Ifirmware/main \
  tests/cpp/safety_decision_test.cpp \
  -o /tmp/carec_safety_decision_test
/tmp/carec_safety_decision_test
```

This directly compiles `firmware/main/safety_decision.h` and verifies zone boundaries plus fail-safe behavior for unavailable or failed detection.

## Run the Python tests

Use an isolated environment:

```bash
python3 -m venv .venv
. .venv/bin/activate
python -m pip install pytest pytest-json-report
pytest tests/obstacle_test.py -v
```

The Python suite covers legacy distance heuristics, zone boundaries, beep timing, fixtures, event format, and mirrored failure-state expectations. It does not replace the portable C++ test.

## CI

[`.github/workflows/test.yml`](../.github/workflows/test.yml) runs both suites when firmware or test inputs change. Project-wide repository checks are defined in [`.github/workflows/project-quality.yml`](../.github/workflows/project-quality.yml).

## Adding tests

Every behavior change should include the lowest-level authoritative test possible:

1. portable C++ test for runtime safety decisions;
2. ROS package unit test for autonomy components;
3. launch or integration test for interfaces;
4. deterministic scenario for system behavior; and
5. machine-readable metrics for milestone evidence.

Future simulation tests must use fixed seeds where practical and report collisions, minimum clearance, stopping distance, intervention reason, latency, and final state.

## Known gap

Most existing Python tests mirror portions of firmware behavior instead of executing embedded code. New safety logic should be extracted into portable C++ modules and tested directly.
