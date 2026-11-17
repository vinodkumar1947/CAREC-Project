# Contributor pilot operating model

Status: Proposed with #53; takes effect only when merged. This supplements governance without appointing anyone or weakening S2/S3 review.

## Responsibilities

| Role | Responsibility | Current coverage |
|---|---|---|
| Product owner | User needs, requirements, priority, acceptance of product behavior | Vinod Kumar |
| Technical maintainer | Refine requirements into tasks, resolve interfaces, review and integrate PRs | Not appointed; project maintainer routes reviews temporarily |
| Contributor | Implement one scoped task, attach verification, update docs | Open participation |
| Independent reviewer | Check implementation and evidence; not the PR author | Must be identified per PR before merge |
| CI | Repeat repository and contract checks | Workflows in `.github/workflows/` |

Until a technical maintainer accepts the role, engineering escalation still reaches the project owner. Automation cannot replace independent review. Do not advertise a staffed review service or a guaranteed turnaround.

## Requirement to implementation

1. Product owner describes the user, trigger, desired behavior, non-goals, priority, and acceptance examples.
2. Technical maintainer identifies interfaces, starting files, dependencies, verification, and safety class.
3. A task becomes `ready` only when its dependencies are merged, a review route is named, and acceptance can be checked.
4. Contributor claims and implements the task publicly. For ready S0/S1 pilot tasks, no private introduction or repeated owner approval is required.
5. CI checks evidence; an independent reviewer reviews the PR. Unanswered design questions stay in the issue and become documentation when resolved.
6. Maintainer merges. Official release designation and physical integration remain with the project owner under existing governance.

## Self-service claims

For ready, unassigned S0/S1 pilot work, the first public claim can begin a small change immediately. Do not change other contributors' assignments. Maintainers resolve duplicate claims and update issue state. Post progress within seven days as a courtesy; after fourteen days without an update, a maintainer may ask whether the issue should be reopened for others. These are coordination guidelines, not an automated access mechanism.

## Definition of Ready

Each issue includes outcome, context, scope, starting files/interfaces, required skills, completed dependencies, checkable acceptance criteria, verification commands/manual checks, submission evidence, safety class, and review route. A dropdown saying Ready does not apply a GitHub label automatically; a maintainer applies the label only after checking the gate.

## Definition of Done

Acceptance evidence passes; relevant CI is green; limitations and docs are updated; an independent review is recorded; code is merged. Closing an issue or opening a PR alone is not completion. Do not auto-close the pilot epic until its two newcomer checks are recorded.

## Initial recruitment gate

- Reference demo and tests merged after independent review.
- Required repository checks pass, including resolution of [traceability blocker #59](https://github.com/vinodkumar1947/CAREC-Project/issues/59).
- Five small tasks with complete briefs and satisfied dependencies.
- Two newcomer walkthroughs using the evidence template.
- Public entry page matches the merged source; GitBook synchronization verified.
- Review route and known limitations visible.

For now, share the branch/PR as an early contributor preview. Do not describe it as a validated release.
