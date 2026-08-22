# Autonomy Architecture

## Separation of responsibility

```text
Destination / user intent
          |
Perception + localization + map
          |
Planner + trajectory controller
          |
Independent safety supervisor
          |
Safe-motion interface
       /       \
Simulator       Owner-only physical adapter
```

The navigation stack may request motion. Only the safety supervisor may publish a command to a platform adapter.

## Planned packages

| Package | Responsibility |
|---|---|
| `carec_interfaces` | Stable messages, services, actions, and status reasons |
| `carec_description` | Geometry, footprint, mass, and drive profiles |
| `carec_simulation` | Gazebo adapters, sensors, worlds, and actors |
| `carec_perception` | Sensor processing and uncertainty |
| `carec_localization` | Pose estimation and health status |
| `carec_navigation` | Planning, control, and recoveries |
| `carec_shared_control` | User-intent arbitration |
| `carec_safety` | Limits, watchdogs, collision monitoring, emergency stop |
| `carec_evaluation` | Scenarios, metrics, reports, and regressions |
| `carec_bringup` | Reproducible launch configurations |

## Non-bypassable safety rules

- Hardware adapters accept only safety-supervisor output.
- Commands expire unless refreshed.
- Stale required sensors and invalid state estimates enter a defined safe state.
- Emergency stop is latched and requires an explicit reset.
- Manual/user authority and arbitration behavior are specified and tested.
- Every intervention records a timestamp and reason code.
- Physical adapters are excluded from default community builds.

## Wheelchair profiles

The simulator will represent differential, mid-wheel, rear-wheel, and other supported kinematics through configuration. A profile describes geometry and motion behavior; it does not certify a physical model.
