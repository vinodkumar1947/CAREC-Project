# Firmware Workstream

Status: Draft
Owner: Firmware Workstream
Last Updated: 2026-08-23
Related Issues: CARE-027, CARE-028
Related ADRs: ADR-0004

Owns MCU communication, sensor drivers, health monitoring, watchdogs, and carefully bounded motor/safety interfaces. Existing SenseCAP code is a warning prototype and useful test input. New code must be hardware-isolated, disabled safely by default, host-testable where practical, and reviewed under the safety classification in [CONTRIBUTING.md](../../CONTRIBUTING.md).
