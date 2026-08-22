# Asynchronous Team Workflow

## Workstreams

| Workstream | Initial size | Scope |
|---|---:|---|
| Simulation and robot model | 3 | Wheelchair profiles, sensors, worlds |
| Safety and testing | 2 | Watchdogs, collision monitor, fault injection |
| Navigation and autonomy | 3 | Localization, planning, shared control |
| Developer experience and status | 2 | Containers, CI, guides, dashboard |

The project owner controls priorities, releases, safety-critical merges, repository access, and physical hardware integration.

## Asynchronous rhythm

- GitHub Issues contain the complete task, acceptance criteria, owner, and test command.
- Contributors post a short issue update when starting, blocked, or handing off.
- Pull requests should be small and normally represent one to three days of work.
- Decisions affecting multiple packages are recorded as architecture decision records.
- A weekly written review summarizes evidence, blockers, and next priorities.

## Review delegation

Workstream reviewers may approve S0–S2 changes appropriate to their experience. The owner retains final merge authority for S3 safety, velocity, release, and physical-adapter changes.

## Avoiding conflicts

- One issue has one accountable owner.
- Announce shared-file changes before editing them.
- Prefer package-local configuration over one large global file.
- Do not mix formatting sweeps with functional changes.
- Rebase or update a branch before requesting final review.

## Access

Give least-privilege repository access. Contributors work through branches and pull requests; direct changes to protected branches and releases are restricted.
