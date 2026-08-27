# Documentation Migration Report

Status: Draft
Owner: Documentation Workstream
Last Updated: 2026-08-23
Related Issues: CARE-046
Related ADRs: TBD

## Existing files discovered

The repository contained root governance/product files; a SenseCAP Watcher firmware implementation and documentation; a dependency-free autonomy reference simulator; tests and traceability; hardware BOM/schematics; contributor and setup guides; weekly reviews; business media; research drafts; CI workflows; issue forms; and archived source prompts. A detailed inventory can be reproduced with `rg --files`.

## Relevant CAREC information preserved

- The existing mission, user-control emphasis, simulation-first direction, physical-integration boundary, MIT license, owner authority, and safety classifications were retained and linked into the new structure.
- `autonomy_ws/carec_simulation`, `evaluation`, `tests`, `scripts`, and `docs/traceability.md` remain intact as implemented evidence.
- The SenseCAP Watcher firmware, hardware-selection records, firmware API/guides, BOM, schematics, and tests remain in place as a warning-device prototype record.
- `PRODUCT.md`, `PROJECT_LOG.md`, `CHANGELOG.md`, `PRIVACY.md`, weekly reviews, research, and business materials were not deleted.

## Where information was moved or linked

No substantive historical file was moved. The numbered documentation hierarchy now links to the active reference simulator, traceability table, legacy architecture, research, and release policy. Root README, roadmap, governance, and contribution guidance were updated in place because they are repository entry points; their prior concepts were retained where compatible and are available in git history.

## Duplicate information

Product direction previously appeared in `README.md`, `PRODUCT.md`, `docs/vision/PROJECT_AIM.md`, and archived comprehensive documentation. Architecture appeared in `docs/ARCHITECTURE.md`, `docs/architecture/AUTONOMY_ARCHITECTURE.md`, and system specifications. Safety and testing appear in both legacy firmware folders and the new numbered system. The new `docs/README.md` states precedence and links rather than deleting these sources.

## Conflicts and unanswered questions

- Earlier material labels SenseCAP Watcher as “ordered/finalized” for an obstacle-warning prototype; it is not an approved autonomy sensor or compute choice.
- Some earlier material contains schedules, prices, performance targets, child-use language, and compatibility implications that need revalidation.
- The former roadmap used a twelve-week schedule and weighted percentage model; the new roadmap uses evidence gates and no unsupported percentage.
- The preserved `ADR-001` has a different numbering scheme and needs disposition.
- Simulator, ROS 2 distribution, sensor suite, motor interface, compute, navigation, inference, and communications remain Proposed.
- Repository naming (`CAREC-Project` versus requested conceptual `carec/`) was preserved to avoid an unnecessary root move.

## Documents requiring owner review

`PRODUCT.md`; `docs/vision/PROJECT_AIM.md`; `docs/status/PROJECT_SCORECARD.md`; legacy `docs/ARCHITECTURE.md`; `docs/specifications/HARDWARE_STATUS_UPDATE.md`; `SENSECAP_WATCHER_HARDWARE_SELECTION.md`; `system_spec.md`; `AUTONOMY_LEVELS.md`; `docs/testing/test_plan.md`; and the eight proposed ADRs.

## Recommended next actions

1. Approve terminology and intended-use boundaries.
2. Triage the initial backlog and create M0 issues/milestone in GitHub.
3. Decide ADR-0001 and ADR-0002 using proofs of concept.
4. Add legacy/front-matter banners consistently after owner review.
5. Configure labels, branch protection, Project views, Discussions, and required reviewers.
6. Add automated Markdown link and Mermaid validation to CI.
