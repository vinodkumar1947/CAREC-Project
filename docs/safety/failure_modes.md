\# Safety Failure Modes



This document defines the failure modes referenced by the requirements and

verification traceability records.



\## HAZ-001 — Collision



A collision may occur when an obstacle is missed or braking begins too late.



Mitigations include footprint-aware stop envelopes, diverse sensing, speed

limits, and fault-injected simulation followed by controlled stopping tests.



\## HAZ-002 — Unexpected Motion



Unexpected motion may occur when a command is stale, corrupt, or issued during

reset.



Mitigations include command freshness checks, bounded commands, and ensuring

that reset produces zero motion.



\## HAZ-003 — Loss of Sensing



Required range data may freeze or become unavailable while the system is

moving.



Mitigations include per-source watchdogs and transition to a conservative

degraded state.



\## HAZ-004 — Localization Failure



The system pose may jump or become invalid during navigation.



Mitigations include confidence gating, goal cancellation, and a safe stop.



\## HAZ-005 — E-Stop Failure



The emergency stop may fail to latch or an unauthorized reset may cause motion.



Mitigations include an independent stop path and a deliberate authorized reset.



\## HAZ-006 — Tip-Over



Excessive speed or turning on a slope or near a threshold may cause a

tip-over.



Mitigations include dynamics limits, slope detection, and speed/turn limits.



\## HAZ-007 — Drop/Stair Event



A forward route may contain an unobserved drop or stair event.



Mitigations include dedicated drop sensing, route constraints, and safe stop

behavior.



\## HAZ-008 — AI Misclassification



A person, obstacle, or doorway may be incorrectly classified by an AI-based

system.



Mitigations include uncertainty handling, a non-AI safety layer, and diverse

evaluation.



\## HAZ-009 — Compute Failure



The main process may hang while the last command persists.



Mitigations include an independent watchdog and command timeout.



\## HAZ-012 — Unauthorized Caregiver Control



A remote actor may issue an unauthorized command.



Mitigations include authentication, consent, local priority, and audit logging.
