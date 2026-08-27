# Integration Testing

Status: Draft
Owner: Testing and ROS 2 Workstreams
Last Updated: 2026-08-23
Related Issues: CARE-041
Related ADRs: ADR-0002

Verify interfaces among nodes and components: message schemas, units, frames, QoS, startup order, lifecycle, timeouts, degraded modes, and shutdown. Use mocks and recorded data; assert both output and diagnostic state. Contract tests should prevent a simulator or driver change from silently altering safety behavior.
