# Getting Started

Status: Draft
Owner: Documentation Workstream
Last Updated: 2026-08-23
Related Issues: CARE-043, CARE-044
Related ADRs: ADR-0001, ADR-0002

1. Read the root README, vision, safety philosophy, roadmap, and contribution policy.
2. Fork or clone the repository and run `./scripts/bootstrap.sh --check`.
3. Run `python3 -m pytest tests/obstacle_test.py tests/unit -v`.
4. Choose a `hardware:not-required`, `status:ready` issue with explicit acceptance criteria.
5. Comment to coordinate, create a correctly named branch, make one focused change, and open a PR.

No hardware is required for ROS 2 nodes, algorithms, simulation assets, AI evaluation, synthetic data, tests, documentation, UI mocks, backend contracts, CI, or safety analysis. Hardware-dependent code must sit behind documented interfaces and mocks.
