# Windows 11 + WSL2 Dev Container Validation

**Status:** Partially Verified
**Issue:** #35
**Platform:** Windows 11 + WSL2 + Ubuntu 24.04
**Last Updated:** 2026-11-02

## 1. Objective

Validate the CAREC development workflow on Windows 11 using WSL2, Ubuntu 24.04, Docker Desktop, and Visual Studio Code Dev Containers.

The objective is to confirm that contributors can use the documented CAREC development environment without installing a separate native-Windows ROS 2 stack.

## 2. Test Environment

| Component | Environment |
| --- | --- |
| Operating System | Windows 11 Home Single Language, Version 25H2 |
| WSL Distribution | Ubuntu 24.04 |
| WSL Version | WSL2 |
| WSL Kernel | 6.18.54.1-microsoft-standard-WSL2 |
| CPU Architecture | x86_64 |
| Available CPUs | 8 |
| Docker Desktop | 4.97.0 |
| Docker Engine | 29.9.2 |
| Visual Studio Code | 1.144.0 |
| Dev Containers Extension | 0.469.0 |
| ROS Distribution | ROS 2 Jazzy |
| Python | 3.12.3 |

## 3. Environment Setup and Container Build

The CAREC repository was opened from the WSL Linux filesystem and accessed through Visual Studio Code using the WSL and Dev Containers extensions.

The initial container build encountered a user and group naming conflict because the ROS Jazzy base image already contains an `ubuntu` user and group.

The Dockerfile was updated to rename the existing user and group to `carec` rather than attempting to create duplicate accounts.

After this change, the Dev Container was rebuilt and opened successfully.

## 4. Validation Results

| Validation | Result | Evidence |
| --- | --- | --- |
| WSL2 and Ubuntu 24.04 | Passed | Ubuntu environment accessed |
| Docker Desktop WSL integration | Passed | Docker client and server available |
| Repository in WSL filesystem | Passed | Workspace opened under `/workspaces/CAREC-Project` |
| VS Code Dev Container | Passed | Container reopened successfully |
| Container build | Passed after Dockerfile fix | Rebuild completed |
| Bootstrap check | Passed | Contributor environment ready |
| ROS 2 Jazzy | Passed | `ROS_DISTRO=jazzy` |
| Unit tests | Passed | 9 tests passed |
| Required obstacle test path | Not available | `tests/obstacle_test.py` does not exist |
| RViz2 GUI | Blocked | WSL display connection failed |
| Full Gazebo robot simulation | Not verified | No CAREC world or launch files available |
| Headless reference simulator | Passed | Simulator smoke test completed |

## 5. Bootstrap Validation

Command:

```sh
./scripts/bootstrap.sh --check
```

Result:

```text
CAREC contributor environment is ready.
Python 3.12.3
ROS 2 jazzy is available.
```

## 6. Unit Test Validation

Command:

```sh
python3 -m pytest tests/unit -v
```

Result:

- 9 tests passed.
- 0 tests failed.

The requested issue command:

```sh
python3 -m pytest tests/obstacle_test.py tests/unit -v
```

could not complete because `tests/obstacle_test.py` is not present in the repository. The existing unit test suite was run separately and passed.

## 7. GUI and Simulation Limitations

### RViz2

RViz2 was available in the container, but the GUI could not connect to the WSL display. The following display error was observed:

```text
qt.qpa.xcb: could not connect to display :0
```

RViz2 graphical operation is therefore not verified.

### Gazebo

Gazebo Sim 8.11.0 was available. A headless Gazebo process remained running until the timeout, but no CAREC world or robot launch assets were available for a full simulation test.

Therefore, full CAREC Gazebo simulation is not verified.

### Reference Simulator

The dependency-free reference simulator smoke test completed successfully and produced the expected sample pose output.

This confirms basic simulator execution only; it is not evidence of physical wheelchair safety.

## 8. Safety and Scope

No physical wheelchair hardware or occupied-wheelchair testing was performed.

Simulation and software testing do not establish physical wheelchair safety. Hardware-in-the-loop, controlled wheelchair testing, and supervised user validation require separate approved protocols and authorization.

## 9. Conclusion

The Windows 11 + WSL2 + Ubuntu 24.04 + Docker Desktop + VS Code Dev Container workflow was successfully established after correcting the Dockerfile user/group conflict.

The bootstrap check and ROS 2 Jazzy availability were confirmed in the rebuilt container. The existing unit test suite passed in the rebuilt container.

The missing obstacle test path and RViz2/Gazebo limitations are documented. The platform should be considered verified only for the capabilities supported by the recorded evidence.
