# ROS 2 Architecture

Status: Proposed
Owner: ROS 2 Workstream
Last Updated: 2026-08-23
Related Issues: TBD
Related ADRs: ADR-0002, ADR-0006

Candidate nodes include input adapter, mode manager, robot state publisher, simulated sensor bridges, localization, perception, fusion, planner/controller, safety supervisor, diagnostics, and platform adapter. Topics, services, actions, QoS, TF frames, lifecycle behavior, namespaces, and timestamps require an interface specification before implementation.

Proposed TF root: `map → odom → base_link`, with sensor frames attached beneath `base_link`. Frame names and authority remain TBD.
