# Contributing to CAREC

CAREC welcomes software, simulation, safety, documentation, accessibility, and research contributions. Normal contributors require no wheelchair hardware.

## Before you begin

Read the [product definition](PRODUCT.md), [project aim](docs/vision/PROJECT_AIM.md), [team workflow](docs/contributors/TEAM_WORKFLOW.md), and [first contribution guide](docs/contributors/FIRST_CONTRIBUTION.md).

This is safety-related assistive technology. Community builds are research software and must not be connected to an occupied wheelchair.

## Simple Git workflow

1. Select an assigned GitHub issue.
2. Create a short branch from `main`, such as `sim/123-doorway-world`.
3. Make one focused change.
4. Run the checks listed in the issue.
5. Open a pull request using the template.
6. Respond to review comments and wait for required checks.
7. The maintainer squash-merges the pull request.

Never push directly to `main`. Do not combine unrelated work in one pull request.

## Definition of done

A task is complete only when:

- its acceptance criteria pass;
- new or changed behavior has tests;
- relevant documentation is updated;
- formatting and automated checks pass;
- safety impact is declared;
- results or screenshots are attached when requested;
- the pull request is reviewed and merged.

## Safety classifications

| Classification | Examples | Approval |
|---|---|---|
| S0 | Documentation, formatting | One reviewer |
| S1 | Simulation assets, dashboards, tooling | Workstream reviewer |
| S2 | Perception, localization, planning | Reviewer plus scenario evidence |
| S3 | Safety supervisor, velocity output, hardware adapter | Owner approval and safety evidence |

Only the project owner may approve S3 changes for a physical release. No contribution may bypass the safety supervisor.

## Hardware boundary

- Simulation adapters are open for normal contribution.
- Physical adapters are disabled by default and owner-controlled.
- Do not add secrets, wheelchair bus captures, personal health information, or identifiable participant data.
- Do not publish instructions that encourage unreviewed motor integration.

## Licensing

By submitting a contribution, you agree to license it under the repository's MIT License. Unless a separate signed agreement says otherwise, contributors retain copyright in their contributions. Repository administration and release authority remain with the project owner.

## Conduct

Be respectful, evidence-driven, and accessible. Discuss technical decisions without dismissing users, caregivers, clinicians, or less-experienced contributors.
