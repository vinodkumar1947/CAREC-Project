# Pilot contribution backlog

Parent: [Contributor Pilot #53](https://github.com/vinodkumar1947/CAREC-Project/issues/53).

The five implementation tasks below are **blocked until the pilot PR is merged**. Each issue contains starting files, skills, scope, acceptance criteria, verification, submission evidence, and a review route. A maintainer checks dependencies before marking a task `status:ready`; this document does not override live issue state.

| Task | Track | Difficulty | Dependency |
|---|---|---|---|
| [#54 — CSV export](https://github.com/vinodkumar1947/CAREC-Project/issues/54) | JavaScript/data export | Beginner | #53 implementation merged |
| [#55 — Keyboard navigation](https://github.com/vinodkumar1947/CAREC-Project/issues/55) | Frontend/accessibility | Intermediate | #53 implementation merged |
| [#56 — Translation dictionary](https://github.com/vinodkumar1947/CAREC-Project/issues/56) | Frontend/documentation | Beginner–intermediate | #53 implementation merged |
| [#57 — Sample scenarios](https://github.com/vinodkumar1947/CAREC-Project/issues/57) | Scenarios/UI | Beginner–intermediate | #53 implementation merged |
| [#58 — JSON report schema](https://github.com/vinodkumar1947/CAREC-Project/issues/58) | Contracts/testing | Intermediate | #53 implementation merged |

## Work possible during review

Newcomers can try the `feature/CARE-053` branch and post sanitized onboarding feedback on #53 using the [validation template](newcomer-validation.md). Record that this is a branch preview, not a release. Independent reviewers can review the pilot PR. No contributor or reviewer is assumed appointed.

Existing ROS environment validation issues [#34](https://github.com/vinodkumar1947/CAREC-Project/issues/34), [#35](https://github.com/vinodkumar1947/CAREC-Project/issues/35), and [#36](https://github.com/vinodkumar1947/CAREC-Project/issues/36) are separate work; check their current state and use current `tests/unit` paths rather than the removed `tests/obstacle_test.py` path in older issue text.

## Planned after this foundation

- Licensed staged video scenarios with explicit annotation instructions.
- A model-adapter proposal with prediction provenance, model/data licensing, uncertainty and evaluation requirements.
- Benchmark slices for occlusion, lighting and camera variation.
- A reproducible hosted demo deployment from an approved commit.

These are roadmap items, not claims of implemented AI. Avoid broad “build all AI” tasks; propose a bounded interface and evaluation first. The robotics simulation backlog remains independent.
