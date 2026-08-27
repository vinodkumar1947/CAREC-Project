# High-Level Architecture

Status: Proposed
Owner: Architecture Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0004, ADR-0008

```mermaid
flowchart TD
  UI["User / Caregiver"] --> HMI["HMI"] --> BM["Mission / Behavior Manager"]
  BM --> MAN["Manual"]
  BM --> AST["Assisted"]
  BM --> AUT["Autonomous"] --> NAV["Navigation"]
  NAV --> PER["Perception"]
  NAV --> LOC["Localization"]
  PER --> FUS["Sensor Fusion"]
  LOC --> FUS
  CAM["Camera"] --> FUS
  LID["LiDAR / range"] --> FUS
  OTH["IMU / encoders / ultrasonic"] --> FUS
  MAN --> SAFE["Safety Supervisor"]
  AST --> SAFE
  NAV --> SAFE
  FUS --> SAFE
  ESTOP["E-stop / watchdog"] --> SAFE
  SAFE --> ADP["Platform Adapter"] --> MOTOR["Motor Controller / Motors"]
```

The safety supervisor is the only path to physical actuation. Higher-level components propose commands; they do not own final motion authority. Exact component boundaries remain subject to ADR review.
