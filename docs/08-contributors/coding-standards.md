# Coding Standards

Status: Draft
Owner: Architecture Workstream
Last Updated: 2026-08-23
Related Issues: CARE-044
Related ADRs: TBD

Prefer clear interfaces, small modules, explicit units and frames, monotonic time for freshness, bounded inputs, deterministic tests, structured diagnostics, and dependency injection for hardware. Python and C++ format/lint tools are TBD. Public interfaces require documentation; safety behavior requires requirement/hazard IDs and tests. Never silently recover from a condition that can create unsafe motion.
