# Failure Modes

Status: Preliminary
Owner: Safety Workstream
Last Updated: 2026-08-23
Related Issues: CARE-037
Related ADRs: ADR-0004, ADR-0008

| Failure | Detection | Preliminary response |
|---|---|---|
| Command/communication timeout | Monotonic freshness watchdog | Zero motion and diagnostic |
| Sensor stale, missing, or implausible | Per-source health and range checks | Degrade or stop according to dependency |
| Compute process crash/hang | Independent heartbeat watchdog | Revoke motion authority |
| Battery undervoltage | Independent power monitor | Controlled stop if feasible; inhibit restart |
| Motor-controller fault | Status/feedback mismatch | Stop via independent approved mechanism |
| Perception/AI uncertainty | Confidence and consistency checks | Do not increase authority; reduce speed/stop |
| Localization/navigation failure | Quality and progress monitor | Cancel goal and stop safely |
| Unexpected acceleration | Command/feedback plausibility | Emergency stop and latch fault |

Actual physical responses are platform-specific and TBD.
