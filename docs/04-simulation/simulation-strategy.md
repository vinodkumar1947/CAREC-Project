# Simulation Strategy

Status: Proposed
Owner: Simulation Workstream
Last Updated: 2026-08-23
Related Issues: CARE-004, CARE-005
Related ADRs: ADR-0001, ADR-0002

All core functions should run against simulated or mocked interfaces. A clean checkout should eventually launch a virtual differential-drive wheelchair, command it, observe odometry/IMU/range data, inject faults, and produce repeatable evidence.

The candidate stack is Ubuntu, ROS 2, Gazebo or an equivalent modern simulator, RViz, Nav2, SLAM, Python, C++, OpenCV, PyTorch, Docker/dev containers, and GitHub Actions. Exact distributions and simulator are **Proposed** pending ADR approval. Fixed seeds, versioned worlds, machine-readable scenario definitions, and recorded metrics are required for repeatability.
