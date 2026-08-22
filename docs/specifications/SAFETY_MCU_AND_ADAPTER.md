# Safety MCU and Wheelchair Adapter Contract

**Status:** architecture contract; no physical implementation is authorized.

## Boundary

High-level autonomy may publish a requested planar velocity. Only an independent
safety controller may publish a bounded command to a model-specific physical
adapter. Community builds terminate at the simulator adapter.

## Logical request

| Field | Unit | Rule |
|---|---|---|
| `sequence` | uint32 | Monotonically increasing; duplicates rejected |
| `stamp_ms` | ms | Command expires after the configured deadline |
| `linear_mps` | m/s | Clamped to the validated profile envelope |
| `angular_rps` | rad/s | Clamped to the validated profile envelope |
| `source` | enum | User, shared-control, navigation, maintenance |
| `crc` | implementation-defined | Transport-integrity check |

## Required safety-controller inputs

- Physical E-stop and reset authorization
- Independent time base and watchdog
- Actual wheel speed or validated equivalent
- Adapter and power health
- Freshness/validity of required perception and localization inputs
- Wheelchair-specific speed, acceleration and stopping envelope

## Mandatory behavior

- No valid fresh command means zero requested motion.
- E-stop is latched and cannot be reset remotely by normal autonomy software.
- Invalid, out-of-order or duplicate commands are rejected.
- Boot, brownout, communication loss and watchdog reset enter zero-motion state.
- Limits are enforced independently of ROS, networking and AI.
- Every intervention exposes a stable reason code.
- Diagnostic/update mode and motion-enabled mode are mutually exclusive.

## Adapter rule

An adapter is approved for one identified wheelchair/controller/firmware,
mechanical installation, configuration and CAREC release. No generic adapter is
implicitly safe for another model. The physical transport and electrical design
remain owner-controlled and absent from default contributor builds.
