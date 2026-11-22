#!/usr/bin/env python3
"""Generate the CAREC project scorecard from checked-in evidence."""

import argparse
import ast
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
STATUS = ROOT / "evaluation/status.json"
SCENARIOS = ROOT / "evaluation/scenarios/baseline.json"
OUTPUT = ROOT / "docs/status/PROJECT_SCORECARD.md"


def generate() -> str:
    subprocess.run(
        [sys.executable, str(ROOT / "scripts/validate_status.py")],
        check=True,
    )

    status = json.loads(STATUS.read_text(encoding="utf-8"))
    scenarios = json.loads(SCENARIOS.read_text(encoding="utf-8"))["scenarios"]

    test_files = sorted((ROOT / "tests").rglob("test_*.py"))
    test_functions = []
    for path in test_files:
        tree = ast.parse(path.read_text(encoding="utf-8"))
        test_functions.extend(
            node.name for node in ast.walk(tree)
            if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef))
            and node.name.startswith("test_")
        )

    lines = [
        "# Project Scorecard",
        "",
        f"**Status date:** {status['updated']}",
        "",
        f"**Current milestone:** {status['milestone'].replace('-', ' ').title()}",
        "",
        f"**Overall simulation-autonomy program:** {status['overall_percent']}% earned",
        "",
        "This scorecard is generated from `evaluation/status.json` and checked-in test and scenario evidence.",
        "",
        "The existing firmware prototype is tracked separately because it does not satisfy the new simulation-autonomy milestones.",
        "",
        "## Earned progress",
        "",
        "| Deliverable | Weight | Earned | Evidence |",
        "|---|---:|---:|---|",
    ]

    for item in status["progress"]:
        lines.append(
            f"| {item['deliverable']} | {item['weight']}% | "
            f"{item['earned']}% | {item['evidence']} |"
        )

    lines.extend([
        "",
        "## Test state",
        "",
        f"- **Simulation regression:** `{status['simulation_regression']}`",
        f"- **Checked-in test files:** {len(test_files)}",
        f"- **Test functions discovered:** {len(test_functions)}",
        "- **Execution result:** A passing CI result is not stored in the checked-in test evidence; consult the latest CI run.",
        "",
        "## Safety scenarios",
        "",
        f"**Baseline scenarios:** {len(scenarios)}",
        "",
        "| ID | Scenario | Expected result |",
        "|---|---|---|",
    ])

    for scenario in scenarios:
        lines.append(
            f"| {scenario['id']} | {scenario['name']} | `{scenario['expected']}` |"
        )

    release = status["latest_release"] or "No simulation-first release recorded"
    lines.extend([
        "",
        "## Release",
        "",
        f"**Latest release:** {release}",
        "",
        "---",
        "",
        "Progress reflects accepted deliverables and evidence, not contributor activity, commits, or lines of code.",
        "",
    ])

    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--check",
        action="store_true",
        help="Fail if the checked-in scorecard differs from generated output",
    )
    args = parser.parse_args()

    expected = generate()

    if args.check:
        if not OUTPUT.exists() or OUTPUT.read_text(encoding="utf-8") != expected:
            raise SystemExit("Project scorecard is stale. Run: python3 scripts/generate_status.py")
        print("Generated project scorecard is up to date.")
    else:
        OUTPUT.parent.mkdir(parents=True, exist_ok=True)
        OUTPUT.write_text(expected, encoding="utf-8")
        print(f"Generated {OUTPUT.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
