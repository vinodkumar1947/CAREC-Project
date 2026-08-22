# CAREC Tests

## Structure

```
tests/
├── obstacle_test.py          # Main integration test suite (run this)
├── conftest.py               # pytest configuration
├── README.md                 # This file
│
├── integration/              # Integration tests (Python, run against firmware output)
│   └── README.md
├── unit/                     # C++ unit tests (future — requires GoogleTest)
│   └── .gitkeep
├── fixtures/                 # Shared test data
│   └── sample_detections.json
└── results/                  # Test run output (gitignored)
```

## Running Tests

### Prerequisites

```bash
pip install pytest pytest-json-report
```

### Run All Tests

```bash
cd /path/to/CAREC-Project
pytest tests/obstacle_test.py -v
```

### Run with JSON Report

```bash
pytest tests/obstacle_test.py -v --json-report --json-report-file=tests/results/latest.json
```

## Test Coverage

| Test File | What It Tests |
|-----------|--------------|
| `obstacle_test.py` | Zone classification logic, distance thresholds (DIST_RED=60, DIST_YELLOW=100), beep pattern mapping, 50-scenario obstacle matrix |

## Key Threshold Values (Must Match Firmware)

| Constant | Value | Defined In |
|----------|-------|-----------|
| `DIST_RED` | 60 cm | `firmware/config/zone_config.h` |
| `DIST_YELLOW` | 100 cm | `firmware/config/zone_config.h` |
| RED test point | 30 cm | mid-zone representative |
| YELLOW test point | 80 cm | mid-zone representative |
| GREEN test point | 120 cm | mid-zone representative |

## Adding New Tests

1. Add test functions to `obstacle_test.py` (or create a new file in `tests/`)
2. Add sample detection data to `fixtures/sample_detections.json`
3. Run `pytest tests/ -v` to verify all pass
4. Commit both the test and the fixture data

## CI/CD

Tests run automatically on every push via GitHub Actions:  
`.github/workflows/test.yml`
