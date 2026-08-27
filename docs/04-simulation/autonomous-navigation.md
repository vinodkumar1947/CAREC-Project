# Autonomous Navigation in Simulation

Status: Proposed
Owner: Navigation Workstream
Last Updated: 2026-08-23
Related Issues: CARE-016 through CARE-019
Related ADRs: ADR-0006

Navigation work proceeds from teleoperation and odometry to mapping, localization, planning, controller tuning, recovery, and supervised goal navigation. Each stage must define the map/world version, initial pose, destination, obstacle motion, timeout, metrics, and safe failure. Nav2 and SLAM Toolbox are candidates, not irreversible choices.
