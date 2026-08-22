#!/usr/bin/env bash
set -euo pipefail

required=(git python3)
missing=()

for command_name in "${required[@]}"; do
  if ! command -v "${command_name}" >/dev/null 2>&1; then
    missing+=("${command_name}")
  fi
done

if ((${#missing[@]})); then
  echo "Missing required commands: ${missing[*]}" >&2
  exit 1
fi

if [[ "${1:-}" == "--check" ]]; then
  echo "CAREC contributor environment is ready."
  python3 --version
  if command -v ros2 >/dev/null 2>&1; then
    ros2 --help >/dev/null
    echo "ROS 2 ${ROS_DISTRO:-unknown} is available."
  else
    echo "ROS 2 is not installed; use the development container for autonomy work."
  fi
  exit 0
fi

echo "Run this repository in its development container, then use:"
echo "  ./scripts/bootstrap.sh --check"
