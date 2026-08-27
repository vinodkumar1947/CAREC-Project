# Contributing to CAREC

CAREC welcomes simulation, robotics, navigation, vision, AI/ML, sensor fusion, firmware, hardware design, mobile, cloud, safety, testing, accessibility, research, and documentation contributions. Most work requires no hardware.

## Start here

1. Read the [vision](docs/01-project-overview/vision.md), [safety philosophy](docs/06-safety/safety-philosophy.md), and [getting-started guide](docs/08-contributors/getting-started.md).
2. Run `./scripts/bootstrap.sh --check` and the tests named in your issue.
3. Select an assigned or `status:ready` issue; ask before starting unscoped work.
4. Create a short-lived branch: `feature/CARE-###`, `fix/CARE-###`, `research/CARE-###`, `docs/CARE-###`, or `simulation/CARE-###`.
5. Open a focused pull request. Do not directly commit feature work to `main`.

## Definition of done

- Acceptance criteria pass and evidence is attached.
- Behavior changes have tests; documentation and traceability are updated.
- Safety, privacy, security, accessibility, and compatibility impacts are declared.
- New dependencies and their licenses are disclosed.
- Relevant CI passes and an independent review is complete.

## Safety classifications

| Class | Examples | Minimum review |
|---|---|---|
| S0 | Documentation and formatting | Reviewer |
| S1 | Tooling, simulation assets, non-motion UI | Workstream reviewer |
| S2 | Perception, localization, planning, shared-control research | Reviewer plus scenario evidence |
| S3 | Safety supervisor, velocity output, firmware release, physical adapter | Safety Reviewer and Project Maintainer |

Community code may propose motion but may never bypass the safety supervisor or directly access physical motors. Physical adapters are disabled by default and owner-controlled. Do not include secrets, personal health information, identifiable participant data, private wheelchair bus captures, or unsafe occupied-testing instructions.

By submitting a contribution, you agree to license it under the repository’s MIT License while retaining copyright unless another signed agreement applies. Follow the [Code of Conduct](CODE_OF_CONDUCT.md) and [governance model](GOVERNANCE.md).
