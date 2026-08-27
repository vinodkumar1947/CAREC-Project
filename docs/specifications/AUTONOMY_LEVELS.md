# CAREC Capability and Claim Levels

This document uses the same three capability levels as the root README and
product requirements. Levels describe increasing motion authority; they are
not claims that the capability is currently implemented or safe for physical
use.

| Level | Capability | Permitted research claim | Evidence gate |
|---|---|---|---|
| Level 1 | Manual drive with independent monitoring | User commands simulated motion; defined faults reach a safe state | Command, timeout, E-stop and feedback tests |
| Level 2 | Assisted drive/shared control | User intent is transparently limited in published scenarios | Closed-loop regression, intervention metrics and safety review |
| Level 3 | Supervised indoor autonomous drive | Navigation works only inside a stated simulation ODD | Repeated mission, recovery and human-override evidence |

Physical validation is tracked separately:

| Stage | Scope | Evidence gate |
|---|---|---|
| P0 | Owner-controlled, unoccupied bench prototype | Electrical, timing, braking, fault and adapter evidence |
| P1 | Supervised occupied research | Separate ethical, regulatory, clinical/human-factors and safety authorization |

Advancement is evidence-based; completing code does not automatically advance
the project. “Autonomous wheelchair,” “safe,” “universal,” and medical-benefit
claims require explicit owner approval and evidence appropriate to the intended
market and use.
