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
    "progress",
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
    items = status["progress"]
    if not isinstance(items, list) or not items:
        raise SystemExit("progress must be a non-empty list")
    if any(not isinstance(item, dict) or not {"deliverable", "weight", "earned", "evidence"} <= item.keys() for item in items):
        raise SystemExit("Each progress item needs deliverable, weight, earned, and evidence")
    if any(type(item["weight"]) is not int or type(item["earned"]) is not int or item["weight"] < 0 or not 0 <= item["earned"] <= item["weight"] or not isinstance(item["evidence"], str) or not item["evidence"].strip() for item in items):
        raise SystemExit("Invalid progress weight, earned value, or evidence")
    if sum(item["weight"] for item in items) != 100:
        raise SystemExit("Progress weights must total 100")
    if sum(item["earned"] for item in items) != progress:
        raise SystemExit("Earned progress must match overall_percent")
    if type(status["safety_scenarios"]) is not int or status["safety_scenarios"] < 0:
        raise SystemExit("safety_scenarios must be a non-negative integer")
    print(f"CAREC status valid: {progress}% — {status['milestone']}")


if __name__ == "__main__":
    main()
