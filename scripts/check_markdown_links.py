#!/usr/bin/env python3
"""Check repository-local links in Markdown files."""

from __future__ import annotations

import re
import sys
from pathlib import Path
from urllib.parse import unquote

ROOT = Path(__file__).resolve().parents[1]
LINK = re.compile(r"(?<!!)\[[^]]*\]\(([^)]+)\)")


def main() -> int:
    failures: list[str] = []
    for document in ROOT.rglob("*.md"):
        if any(part in {".git", "build", "managed_components", ".cache"} for part in document.parts):
            continue
        text = document.read_text(encoding="utf-8", errors="replace")
        for raw in LINK.findall(text):
            destination = raw.strip().split()[0].strip("<>")
            if destination.startswith(("http://", "https://", "mailto:", "#")):
                continue
            path_text = unquote(destination.split("#", 1)[0])
            if not path_text:
                continue
            target = (document.parent / path_text).resolve()
            if not target.exists():
                failures.append(f"{document.relative_to(ROOT)} -> {destination}")
    if failures:
        print("Broken local Markdown links:")
        print("\n".join(f"- {item}" for item in failures))
        return 1
    print("All local Markdown links resolve.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
