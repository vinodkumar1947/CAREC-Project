# Hardware Architecture

Status: Proposed
Owner: Hardware Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0003, ADR-0004, ADR-0005

Conceptual layers are: user input and feedback; high-level compute; independent safety controller; isolated platform adapter; manufacturer motor controller; sensors; power and emergency-stop chain. Interfaces shall fail safe, expose health, and prevent general application software from writing directly to motors.

The repository contains a real SenseCAP Watcher warning prototype, documented under `docs/specifications`. It is research input, not the finalized autonomy compute or sensor suite. Voltage, current, EMC, ingress, mounting, braking, and connector requirements are TBD per wheelchair.
