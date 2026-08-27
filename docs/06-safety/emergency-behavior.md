# Emergency Behavior

Status: Preliminary
Owner: Safety Workstream
Last Updated: 2026-08-23
Related Issues: CARE-038
Related ADRs: ADR-0004

Emergency stop, manual override, and caregiver intervention must have documented priority, latency, feedback, latching, reset, and failure behavior. Preliminary priority is: independent physical safety path; user stop/manual override; local safety supervisor; mode manager; remote requests. A stop resets motion output to zero, cancels active goals, records a non-sensitive reason, and remains latched. Reset requires authorization and confirmation of healthy inputs and must not resume the previous command.
