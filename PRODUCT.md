# CAREC Product Definition

**Status:** approved research baseline, version 0.1

**Owner:** Vinod Kumar

**Review trigger:** any change to users, environment, control authority, or claims

## Intended use

CAREC is a research-stage, simulation-first platform for studying indoor
powered-wheelchair hazard awareness and shared control.  In its first target
capability, the user supplies motion intent and a separately testable safety
supervisor may limit or reject an unsafe simulated command.

## Intended users and stakeholders

- Primary user: a powered-wheelchair user who retains driving authority.
- Configuration/support: trained caregiver or service person.
- Clinical stakeholders: occupational and physical therapists and
  rehabilitation engineers.
- Contributors: software engineers working without physical hardware.

Wheelchair users must be represented in requirements and usability decisions.

## Initial operating design domain

- Indoor, mapped, pedestrian environments in simulation.
- Low-speed differential-drive wheelchair profiles.
- Static obstacles first; moving people are a later scenario set.
- Supervised research only.

## Non-use and prohibited claims

CAREC is not approved for occupied, unsupervised, outdoor-road, medical, or
emergency use. Community software must not connect to motors. Simulation
results do not establish clinical safety. CAREC does not claim compatibility
with every wheelchair; each physical combination requires a controlled adapter
and separate validation.

## Product objective

The first product evidence target is **Level 2 assisted drive/shared control in simulation**: user
commands pass through a non-bypassable safety supervisor, unsafe commands stop
with a reason code, and every defined injected failure reaches a deterministic
safe state.

## Success measures

- Zero collisions in the published baseline regression set.
- 100% of defined stale-command, stale-sensor, invalid-localization and E-stop
  faults produce a zero-motion output.
- Safety decisions include stable reason codes and test evidence.
- A clean contributor environment runs all non-hardware checks with one command.

These are engineering gates, not medical performance claims.
