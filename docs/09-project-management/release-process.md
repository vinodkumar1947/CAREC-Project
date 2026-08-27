# Release Process

Status: Proposed
Owner: Project Maintainer
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: TBD

1. Define scope, supported configuration, known limitations, and release acceptance criteria.
2. Freeze scope; pass CI, traceability, dependency/license, security, safety, and documentation checks.
3. Reproduce scenario evidence from a clean environment and review unresolved hazards.
4. Prepare changelog, upgrade/rollback notes, artifacts, checksums, and release notes.
5. Obtain required workstream, safety, and maintainer approvals; tag and publish.
6. Monitor issues and follow [SECURITY.md](../../SECURITY.md) for vulnerabilities.

Simulation, firmware, HIL, and physical releases are distinct. A simulation tag never authorizes wheelchair use. See also the preserved [release policy](../RELEASE_POLICY.md).
