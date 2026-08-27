# Communications

Status: Proposed
Owner: Architecture and Security Workstreams
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0008

Internal ROS 2 transport, MCU links, mobile connectivity, and optional cloud telemetry have different trust and timing needs. Motion-critical communication shall be local, bounded, authenticated where applicable, monitored by watchdogs, and safe on timeout. Remote services must be optional and may not be required to stop safely. Protocol, serialization, rate, retry, security, and degraded behavior remain TBD per interface.
