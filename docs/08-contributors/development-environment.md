# Development Environment

Status: Proposed
Owner: Developer Experience Workstream
Last Updated: 2026-08-29
Related Issues: CARE-004, CARE-043
Related ADRs: ADR-0001, ADR-0002

CAREC uses **Ubuntu 24.04 LTS + ROS 2 Jazzy** as its canonical runtime, but contributors do not need an Ubuntu laptop. Windows, macOS, and Ubuntu contributors should use the same repository-managed Dev Container wherever practical.

## One CAREC environment on three host operating systems

```text
Ubuntu 24.04 ─┐
Windows 11 ───┼──> Git + VS Code + Docker/WSL2 as applicable
macOS ────────┘                    |
                                  v
                       CAREC Dev Container
                       Ubuntu / ROS 2 Jazzy
                       Gazebo / Nav2 / SLAM
                       C++ / Python / tools
                                  |
                                  v
                         Build -> Test -> Simulate
```

The host operating system is only the entry point. The project container is the normal contributor runtime. This reduces platform drift and keeps local development close to CI.

## Common workflow after host setup

After completing the one-time host-specific prerequisites below, every contributor should use the same workflow:

```bash
git clone https://github.com/vinodkumar1947/CAREC-Project.git
cd CAREC-Project
code .
```

In VS Code choose **Dev Containers: Reopen in Container**. The repository's `.devcontainer/devcontainer.json` builds the CAREC container and automatically runs:

```bash
./scripts/bootstrap.sh --check
```

A successful environment reports that the CAREC contributor environment is ready and that ROS 2 Jazzy is available.

> The current container is an early project baseline. Gazebo GUI and GPU acceleration must be validated per host before CAREC promises identical graphical behavior across all platforms. Headless builds and automated tests should remain the portable baseline.

## Ubuntu 24.04 LTS — reference host

Ubuntu 24.04 is the reference host for CAREC, CI parity, advanced robotics development, future sensor integration, hardware-in-the-loop work, and native troubleshooting.

### One-time host setup

Install Git, Docker Engine (or a supported Docker desktop environment), VS Code, and the VS Code **Dev Containers** extension. Then clone CAREC and reopen it in the Dev Container using the common workflow above.

### Native Ubuntu option

Maintainers and hardware engineers may also install ROS 2 Jazzy and the supported Gazebo/ROS packages natively when direct hardware, graphics, timing, or low-level debugging requires it. Native installation is an advanced/reference path; ordinary contributors should prefer the project container so dependency versions remain consistent.

## Windows 11 — WSL2 path

CAREC does not maintain a separate native-Windows ROS/Gazebo stack. Windows contributors should use Linux through WSL2.

### One-time host setup

1. Enable/install WSL2.
2. Install an Ubuntu 24.04 WSL distribution.
3. Install Docker Desktop and enable its WSL2 integration.
4. Install VS Code on Windows.
5. Install the VS Code **WSL** and **Dev Containers** extensions.
6. Configure Git inside the Ubuntu/WSL environment.

Keep the CAREC checkout inside the Linux filesystem for normal robotics development, for example under the contributor's WSL home directory, rather than a Windows-mounted project directory when avoidable.

From the Ubuntu WSL terminal:

```bash
git clone https://github.com/vinodkumar1947/CAREC-Project.git
cd CAREC-Project
code .
```

VS Code should open the WSL workspace. Then choose **Dev Containers: Reopen in Container**. From this point, use the same CAREC commands and workflow as Ubuntu contributors.

```text
Windows 11
   -> WSL2 / Ubuntu 24.04
      -> Docker / CAREC Dev Container
         -> ROS 2 Jazzy + CAREC
```

WSLg may provide Linux GUI support, but Gazebo/RViz graphics behavior must be validated by CAREC before being treated as a release requirement. Headless tests are the required portable baseline.

## macOS — container path

macOS contributors can work on CAREC code, ROS packages, algorithms, tests, documentation, safety tooling, and many headless simulation tasks without replacing macOS.

### One-time host setup

Install Git (or Apple's command-line developer tools providing Git), Docker Desktop, VS Code, and the VS Code **Dev Containers** extension.

Then:

```bash
git clone https://github.com/vinodkumar1947/CAREC-Project.git
cd CAREC-Project
code .
```

Choose **Dev Containers: Reopen in Container** and use the same CAREC workflow.

### Apple Silicon note

M-series Macs use ARM64 processors. CAREC must not claim that every x86_64 robotics or GPU package works identically on ARM64. Contributors can still perform a large amount of C++, Python, ROS, unit-test, documentation, algorithm, and headless work. Platform-specific simulation limitations should be recorded rather than worked around with undocumented local changes.

If a required simulator or GPU workload is not supported adequately on a Mac, use a remote Ubuntu workstation or CI environment instead of changing the CAREC dependency baseline.

```text
macOS
   -> Docker Desktop
      -> CAREC Dev Container
         -> ROS 2 Jazzy + CAREC
```

NVIDIA Isaac Sim is not part of the standard Mac contributor environment. CAREC's future Isaac Sim/high-end GPU work should use a supported Ubuntu + NVIDIA environment.

## What should be identical everywhere

The following project operations should eventually be host-independent because they execute inside the CAREC environment:

- dependency/bootstrap verification;
- workspace build;
- formatting and linting;
- C++ and Python unit tests;
- headless simulation smoke tests;
- scenario/regression tests;
- ROS package structure and interfaces;
- logs and test artifacts;
- pull-request validation.

Graphical simulation, GPU acceleration, USB/sensor passthrough, hardware-in-the-loop, real-time behavior, and vendor-specific drivers may remain host-dependent and should be tested on the Ubuntu reference platform.

## Verification

Inside the CAREC Dev Container run:

```bash
./scripts/bootstrap.sh --check
python3 -m pytest tests/obstacle_test.py tests/unit -v
```

Before a platform is advertised as fully supported for simulation, CAREC should also verify its standard headless simulator smoke test and record the result in CI or the platform-support documentation.

## Support policy

| Host | Code / unit tests | Headless CAREC | Gazebo/RViz GUI | Hardware/HIL | Isaac Sim |
|---|---|---|---|---|---|
| Ubuntu 24.04 x86_64 | Supported target | Supported target | Reference target | Reference target | Future supported configuration |
| Windows 11 + WSL2 | Supported target | Supported target | Validate before guarantee | Use Ubuntu for final validation | Use Ubuntu/NVIDIA |
| macOS Intel | Supported target | Validate | Best effort | Use Ubuntu for final validation | Not standard |
| macOS Apple Silicon | Supported target | Validate ARM64 | Best effort | Use Ubuntu for final validation | Not standard |

The words **supported target** describe the intended project contract. A target becomes verified only when the corresponding automated or documented smoke test exists and passes on that platform.
