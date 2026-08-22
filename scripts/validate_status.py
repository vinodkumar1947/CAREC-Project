#!/usr/bin/env python3
"""Validate the repository-backed CAREC project status record."""

import json
from pathlib import Path


STATUS_PATH = Path(__file__).resolve().parents[1] / "evaluation" / "status.json"
REQUIRED_KEYS = {
    "schema_version",
    "updated",
    "milestone",
    "overall_percent",
    "autonomy_build",
    "simulation_regression",
    "safety_scenarios",
    "latest_release",
}


def main() -> None:
    status = json.loads(STATUS_PATH.read_text(encoding="utf-8"))
    missing = REQUIRED_KEYS - status.keys()
    if missing:
        raise SystemExit(f"status.json is missing keys: {sorted(missing)}")
    progress = status["overall_percent"]
    if not isinstance(progress, int) or not 0 <= progress <= 100:
        raise SystemExit("overall_percent must be an integer from 0 to 100")
    if status["safety_scenarios"] < 0:
        raise SystemExit("safety_scenarios cannot be negative")
    print(f"CAREC status valid: {progress}% — {status['milestone']}")


if __name__ == "__main__":
    main()
