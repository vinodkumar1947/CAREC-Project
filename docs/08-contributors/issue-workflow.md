# Issue Workflow

Status: Draft
Owner: Project Maintainer
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: TBD

```mermaid
flowchart LR
  B["Backlog"] --> R["Ready"] --> A["Assigned"] --> I["In Progress"] --> P["Pull Request"] --> V["Review"] --> T["Testing"] --> D["Done"]
  I --> X["Blocked"] --> R
  V --> I
  T --> I
```

An issue becomes Ready only when scope, deliverable, acceptance criteria, dependencies, hardware need, safety class, workstream, and owner/mentor are clear. Status comes from the GitHub Project; avoid duplicating it in comments. Close only after merge and evidence.
