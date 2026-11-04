# Issue #16: Preliminary Environment Checks

## Test environment

- OS: Windows 11 Home, build 26200.9711
- CPU: AMD Ryzen 3 7320U with Radeon Graphics
- CPU architecture: AMD64
- CPU cores: 4
- Installed RAM: 6,255,276,032 bytes (approximately 5.83 GiB)
- WSL version: 3.0.3.0
- WSL kernel: 6.18.54.1-1
- Linux distribution: Ubuntu 24.04.5 LTS
- Python: 3.12.3

## Checks completed

- Confirmed WSL 2 is operational.
- Installed and launched Ubuntu 24.04.5 LTS under WSL 2.
- Confirmed the Linux user account and Python installation.
- Checked for the ROS 2 CLI in Ubuntu; `ros2` was not found.
- Checked for Docker; the Docker CLI is not installed.

## Results and limitations

The Windows, WSL 2, and Ubuntu environment is operational. Docker is not installed, so the repository's ROS 2 Jazzy development container has not been built or run.

The computer has approximately 5.83 GiB of RAM. This is below Docker Desktop's documented 8 GB Windows requirement and may limit testing on this machine.

## Pending verification

- Build the development container from a clean clone.
- Run `./scripts/bootstrap.sh --check` inside the container.
- Run a minimal ROS 2 command inside the container.
- Record actual build time, resource usage, errors, and reproducible fixes.

## Conclusion

Preliminary host-environment checks are complete. CPU-only development-container compatibility remains unverified pending a successful container build and runtime test on a suitable system.
