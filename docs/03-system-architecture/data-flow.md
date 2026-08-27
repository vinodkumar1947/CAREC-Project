# Data Flow

Status: Proposed
Owner: Architecture Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0008

```mermaid
sequenceDiagram
  participant U as User/HMI
  participant B as Behavior
  participant E as Estimation
  participant S as Safety Supervisor
  participant A as Simulator/Platform Adapter
  U->>B: Mode and intent
  E->>B: Pose, obstacles, confidence
  B->>S: Proposed velocity + timestamp
  E->>S: Independent health/safety inputs
  S->>S: Validate freshness, limits, faults, E-stop
  alt safe
    S->>A: Bounded safe-motion command
  else unsafe or uncertain
    S->>A: Safe-state command + reason
  end
  A-->>S: Actuator/odometry status
  S-->>U: Mode, intervention, fault
```

Logs must avoid personal data by default and preserve enough timestamps and configuration identifiers to reproduce results.
