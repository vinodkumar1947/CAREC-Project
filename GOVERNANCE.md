# CAREC Governance

Status: Draft

Owner: Project Maintainer

Last Updated: 2026-08-23

Related Issues: TBD

Related ADRs: TBD

## Roles

### Project Maintainer

Administers the repository, appoints roles, resolves final scoped architecture decisions, accepts residual risk, approves releases, and controls physical integration. Vinod Kumar currently holds this role.

### Workstream Lead

Maintains a technical area, prepares scoped issues, coordinates interfaces, reviews evidence, and escalates cross-cutting or safety decisions. Appointment does not authorize physical deployment.

### Contributor

Works on assigned or maintainer-approved issues through short-lived branches and pull requests. Contributors follow documented interfaces, tests, conduct, security, privacy, and safety boundaries.

### Reviewer

Independently checks correctness, maintainability, acceptance criteria, tests, documentation, dependencies, and compatibility. Authors do not provide their own sole approval.

### Safety Reviewer

Reviews changes affecting motion, actuation, perception used for intervention, emergency behavior, safety requirements, hazards, or validation. Safety-critical physical changes still require Project Maintainer approval.

## Decisions and permissions

Routine reversible decisions may be made within an approved issue. Cross-workstream, long-lived, costly, safety-relevant, or interface-breaking decisions use an [ADR](docs/11-decisions/README.md). Evidence and documented trade-offs guide discussion; the Project Maintainer makes the final scoped decision.

Contributors work through branches and pull requests. Critical files should use GitHub branch protection, required checks, `CODEOWNERS`, and at least one independent review. Least-privilege access is preferred. Direct feature commits to `main` are prohibited.

Safety classes remain S0 documentation, S1 tooling/simulation assets, S2 perception/localization/planning, and S3 safety/velocity/physical adapter/release. S3 changes require safety review and maintainer approval.

## Releases, continuity, and intellectual property

Only the Project Maintainer may designate an official CAREC release or authorize physical integration. Before external pilots, document backup release administration and incident response. Contributions are accepted under [CONTRIBUTING.md](CONTRIBUTING.md) and the MIT License unless a separate written agreement applies; repository access does not transfer copyright or trademark rights.
