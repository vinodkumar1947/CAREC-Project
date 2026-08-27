# Wheelchair Model

Status: Proposed
Owner: Simulation and Hardware Workstreams
Last Updated: 2026-08-23
Related Issues: CARE-006, CARE-007
Related ADRs: ADR-0004

M0 needs a parameterized differential-drive URDF/SDF with base, wheels, caster representation, collision geometry, inertial properties, and sensor mounts. Dimensions, mass, center of gravity, acceleration, braking, and wheel dynamics are TBD and shall not be presented as a real chair specification. The model must expose configuration and include validation tests for spawn, transforms, collision shape, and commanded motion.
