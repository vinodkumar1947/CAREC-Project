# Emergency Stop Requirements

Status: Proposed
Owner: Safety Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0004

- **REQ-ESTOP-001:** Emergency stop shall preempt all motion modes.
- **REQ-ESTOP-002:** The stopped state shall latch until an authorized, deliberate reset.
- **REQ-ESTOP-003:** Reset shall not itself create motion.
- **REQ-ESTOP-004:** Software stop, physical stop, power fault, and communication loss behaviors shall be separately specified and tested.
- **REQ-ESTOP-005:** Stop latency and stopping distance limits are TBD and must be verified per platform.
