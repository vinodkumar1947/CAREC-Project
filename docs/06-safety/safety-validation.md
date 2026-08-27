# Safety Validation

Status: Preliminary
Owner: Safety Workstream
Last Updated: 2026-08-23
Related Issues: CARE-039 through CARE-042
Related ADRs: TBD

Every safety claim requires a requirement, hazard/control link, test method, configuration, evidence artifact, reviewer, and result. Evidence advances from unit/component tests to ROS 2 integration, simulation fault injection, HIL, controlled unoccupied wheelchair tests, and only then separately approved supervised user validation. Passing simulation never substitutes for hardware or human validation. Unresolved critical hazards block release or promotion.
