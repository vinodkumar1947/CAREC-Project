# CAREC Documentation

This index separates the active simulation-first program from the retained firmware-prototype record.

## Current program

| Document | Purpose |
|---|---|
| [Project aim](vision/PROJECT_AIM.md) | Human outcome, engineering outcome, and boundaries |
| [Roadmap](../ROADMAP.md) | Twelve-week CAREC Sim 0.1 program |
| [Autonomy architecture](architecture/AUTONOMY_ARCHITECTURE.md) | Safety boundary and planned ROS 2 packages |
| [Project scorecard](status/PROJECT_SCORECARD.md) | Evidence-backed program progress |
| [First contribution](contributors/FIRST_CONTRIBUTION.md) | Safe onboarding exercise |
| [Team workflow](contributors/TEAM_WORKFLOW.md) | Workstreams, review, access, and asynchronous coordination |
| [Contribution policy](../CONTRIBUTING.md) | Git workflow, definition of done, safety levels, licensing |

## Legacy firmware prototype

Documents under `api/`, `guides/`, `safety/`, `specifications/`, and the root-level firmware architecture describe the earlier SenseCAP obstacle-warning prototype. They are retained as research evidence and do not define the current autonomy roadmap.

Important limitations:

- The firmware does not control wheelchair motion.
- Its monocular distance estimates are not physically validated for deployment.
- Old schedules, prices, production claims, and compatibility claims are historical.
- No legacy document authorizes unsupervised or occupied-wheelchair use.
- The root README, current roadmap, project aim, and autonomy architecture take precedence.

## Research and stakeholder material

Research drafts and business media are retained for reference. Their claims must be revalidated before reuse in public presentations or publications.
