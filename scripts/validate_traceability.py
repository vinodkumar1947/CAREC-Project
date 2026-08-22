#!/usr/bin/env python3
"""Fail CI when traceability references are missing from repository evidence."""

import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TRACE = (ROOT / "docs/traceability.md").read_text(encoding="utf-8")
HAZARDS = (ROOT / "docs/safety/failure_modes.md").read_text(encoding="utf-8")
TEST_TEXT = "\n".join(
    path.read_text(encoding="utf-8")
    for path in sorted((ROOT / "tests").rglob("*"))
    if path.is_file() and path.suffix in {".py", ".cpp"}
)


def main() -> None:
    requirements = set(re.findall(r"REQ-[A-Z]+-\d{3}", TRACE))
    linked_hazards = set(re.findall(r"HAZ-\d{3}", TRACE))
    known_hazards = set(re.findall(r"HAZ-\d{3}", HAZARDS))
    test_names = set(re.findall(r"`(test_[A-Za-z0-9_\[\]-]+)", TRACE))
    scenarios = json.loads(
        (ROOT / "evaluation/scenarios/baseline.json").read_text(encoding="utf-8")
    )["scenarios"]

    if not requirements:
        raise SystemExit("No requirements found in traceability table")
    missing_hazards = linked_hazards - known_hazards
    if missing_hazards:
        raise SystemExit(f"Unknown linked hazards: {sorted(missing_hazards)}")
    missing_tests = {name.split("[")[0] for name in test_names if name.split("[")[0] not in TEST_TEXT}
    if missing_tests:
        raise SystemExit(f"Referenced tests not found: {sorted(missing_tests)}")
    scenario_ids = [item["id"] for item in scenarios]
    if len(scenario_ids) != len(set(scenario_ids)):
        raise SystemExit("Scenario IDs must be unique")
    print(
        f"Traceability valid: {len(requirements)} requirements, "
        f"{len(linked_hazards)} linked hazards, {len(scenarios)} scenarios"
    )


if __name__ == "__main__":
    main()
