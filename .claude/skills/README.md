# Project-installed Claude Code Skills

These skills are committed to the repo so every contributor working on CAREC with Claude Code has them available automatically. They live at `.claude/skills/` (project scope) — no install step required.

## How skills work

Each skill is a directory containing a `SKILL.md` with frontmatter (`name`, `description`, optional `allowed-tools`). Claude Code reads the descriptions and invokes a skill when its description matches the task at hand. You don't activate them manually; they're context-triggered.

## What's installed

| Skill | Purpose | When it activates |
|---|---|---|
| [`sensecap-watcher`](./sensecap-watcher/SKILL.md) | W1-A firmware specialist: SSCMA, PCA9535, ES8311, tf_module_ops, ESP-IDF build | SSCMA, sscma_client, tf_module, tf_event_post, PCA9535, ES8311, Himax, HX6538, WE2, SenseCraft detection |
| [`gpio-config`](./gpio-config/SKILL.md) | GPIO/I²C/SPI/UART/PWM pin assignment for ESP32 + RPi | When wiring sensors, picking pins on the W1-A, debugging strapping/boot pin conflicts, sdkconfig pin questions |
| [`cpp-pro`](./cpp-pro/SKILL.md) | Modern C++20/23, templates, performance, CMake | When writing/refactoring firmware C++ (`firmware/main/*.h`, `*.cpp`) |
| [`python-pro`](./python-pro/SKILL.md) | Python 3.11+, type hints, async, pytest, mypy strict | When touching `tools/calibration/`, `tools/log_parser/`, `tests/` |
| [`code-reviewer`](./code-reviewer/SKILL.md) | Broad-scope code review (correctness, perf, maintainability) | PR review, audits, refactor suggestions |
| [`secure-code-guardian`](./secure-code-guardian/SKILL.md) | OWASP Top 10, input validation, encryption, JWT/OAuth | OTA cert handling, BLE pairing/auth, WiFi config security |
| [`architecture-designer`](./architecture-designer/SKILL.md) | System design, ADRs, scalability planning | Big-picture firmware architecture, ADRs in `docs/specifications/` |
| [`deep-research`](./deep-research/SKILL.md) | Multi-source research workflow (codebase + web) | Investigating SSCMA APIs, datasheet hunts, deep technical dives |

## Sources

- `gpio-config` — [claudius-ars/embedded-agent-skills](https://github.com/claudius-ars/embedded-agent-skills) (MIT)
- `cpp-pro`, `python-pro`, `code-reviewer`, `secure-code-guardian`, `architecture-designer` — [Jeffallan/claude-skills](https://github.com/Jeffallan/claude-skills) (MIT)
- `deep-research` — [larry-syatech/claude-skills](https://github.com/larry-syatech/claude-skills)

## Skills explicitly NOT installed (with reason)

- `larry-syatech/build-with-esp-idf` — Windows/MSYS-only; activates on every `idf.py` mention and would give wrong advice on macOS
- `laurigates/esp-idf-setup` (skillsmp) — Docker + `just` monorepo pattern; conflicts with the local IDF flow used here. Also the marketplace returned a *reconstruction* of the SKILL.md, not the raw file — unsafe to install
- 25 framework skills from Jeffallan (Flutter, Django, K8s, Vue, React, …) — not in CAREC's stack
- `wedsamuel1230/SFT` — has no SKILL.md; it's hardware/Arduino code, not a Claude skills repo

## Updating

Skills are pinned by `git clone`-snapshot. To refresh, re-clone the upstream repo and copy the `SKILL.md` (and any `references/`, `scripts/`) over the existing directory. Bump versions live in each skill's frontmatter.

## Out-of-scope skills

If you need something not listed here (e.g. for a future React Native caregiver app), add the skill directory under `.claude/skills/<name>/` with a `SKILL.md` and commit it.
