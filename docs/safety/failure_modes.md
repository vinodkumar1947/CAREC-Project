# Failure Modes and Hazards

## List of Known Hazards

- HAZ-001: Forward obstacle detected within the configured safety distance may require an emergency stop.
- HAZ-002: Command processing delay greater than 250 ms may result in unsafe or unexpected motion behavior.
- HAZ-003: Required sensor data older than 250 ms may result in unsafe or unexpected motion behavior.
- HAZ-004: Invalid localization data may result in unsafe or unexpected motion behavior.
- HAZ-005: E-stop state may fail to remain latched until an authorized reset.
- HAZ-006: Commands exceeding configured linear or angular limits may result in unsafe motion.
- HAZ-007: Detector failure or processing-loop overrun may produce degraded-health or stale-timestamp conditions.
- HAZ-008: Operational build may permit an unintended network OTA capability.
- HAZ-009: BLE interface may expose an unauthenticated command characteristic.
- HAZ-012: Physical adapter behavior may vary by model and may not meet the specified safety constraints.
