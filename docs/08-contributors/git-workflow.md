# Git Workflow

Status: Approved
Owner: Project Maintainer
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: TBD

Use an up-to-date branch from `main`: `feature/CARE-###`, `fix/CARE-###`, `research/CARE-###`, `docs/CARE-###`, or `simulation/CARE-###`. Keep commits reviewable, avoid unrelated formatting, re-run issue checks, and open a PR referencing the issue. Do not force-push shared branches or commit feature development directly to `main`. Maintainers normally squash-merge after review and CI.
