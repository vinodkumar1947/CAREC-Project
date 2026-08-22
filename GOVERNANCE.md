# CAREC Governance

## Authority

Vinod Kumar is project owner, product owner, release authority, trademark
authority and sole physical-integration authority. Maintainer permissions do
not authorize physical deployment or product claims.

## Decisions

- Routine S0/S1 work may be reviewed by an assigned workstream reviewer.
- S2 work requires scenario evidence and an independent reviewer.
- S3 safety, command, adapter, firmware-release and physical changes require
  owner approval.
- Intended-use, risk-acceptance, licensing and public product claims are owner
  decisions recorded in version control or an architecture decision record.

Maintainers disclose conflicts of interest. Technical disagreement is resolved
with requirements, evidence and documented trade-offs; the owner makes the
final scoped decision.

## Access and continuity

Contributors receive least-privilege access. Protected branches, required
reviews and CI must be configured in GitHub; `CODEOWNERS` alone is not an
enforcement mechanism. At least one documented backup release procedure should
exist before external pilots to reduce single-person operational risk.

## Intellectual property

Contributions are accepted under `CONTRIBUTING.md` and the MIT License unless a
separate written agreement applies. Repository administration does not by
itself transfer contributor copyright. Trademark rights and the right to call a
build an “official CAREC release” remain with the project owner.
