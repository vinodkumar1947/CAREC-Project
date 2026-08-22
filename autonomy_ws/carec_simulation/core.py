"""Deterministic 2-D reference model used by contributor and CI tests.

This module is deliberately dependency-free.  It defines behavior expected of
future ROS 2 implementations; it is not a physical wheelchair controller.
"""

from dataclasses import dataclass
from math import cos, sin


@dataclass(frozen=True)
class Command:
    linear_mps: float
    angular_rps: float
    stamp_ms: int


@dataclass(frozen=True)
class Pose:
    x_m: float = 0.0
    y_m: float = 0.0
    yaw_rad: float = 0.0


@dataclass(frozen=True)
class SafetyInput:
    now_ms: int
    command: Command
    nearest_obstacle_m: float
    sensor_stamp_ms: int
    localization_valid: bool = True
    emergency_stop: bool = False


@dataclass(frozen=True)
class SafetyOutput:
    command: Command
    allowed: bool
    reason: str


class SafetySupervisor:
    """Reference safety envelope with explicit fail-safe reason codes."""

    def __init__(
        self,
        max_linear_mps: float = 0.6,
        max_angular_rps: float = 1.0,
        stop_distance_m: float = 0.6,
        command_timeout_ms: int = 250,
        sensor_timeout_ms: int = 250,
    ) -> None:
        self.max_linear_mps = max_linear_mps
        self.max_angular_rps = max_angular_rps
        self.stop_distance_m = stop_distance_m
        self.command_timeout_ms = command_timeout_ms
        self.sensor_timeout_ms = sensor_timeout_ms
        self._estop_latched = False

    def reset_emergency_stop(self, physically_authorized: bool) -> bool:
        if physically_authorized:
            self._estop_latched = False
        return not self._estop_latched

    def evaluate(self, state: SafetyInput) -> SafetyOutput:
        stop = Command(0.0, 0.0, state.now_ms)
        if state.emergency_stop:
            self._estop_latched = True
        if self._estop_latched:
            return SafetyOutput(stop, False, "ESTOP_LATCHED")
        if state.now_ms - state.command.stamp_ms > self.command_timeout_ms:
            return SafetyOutput(stop, False, "COMMAND_STALE")
        if state.now_ms - state.sensor_stamp_ms > self.sensor_timeout_ms:
            return SafetyOutput(stop, False, "SENSOR_STALE")
        if not state.localization_valid:
            return SafetyOutput(stop, False, "LOCALIZATION_INVALID")
        if state.command.linear_mps > 0 and state.nearest_obstacle_m <= self.stop_distance_m:
            return SafetyOutput(stop, False, "OBSTACLE_STOP")

        linear = max(-self.max_linear_mps, min(self.max_linear_mps, state.command.linear_mps))
        angular = max(-self.max_angular_rps, min(self.max_angular_rps, state.command.angular_rps))
        limited = linear != state.command.linear_mps or angular != state.command.angular_rps
        safe = Command(linear, angular, state.now_ms)
        return SafetyOutput(safe, True, "LIMIT_APPLIED" if limited else "OK")


class WheelchairSim:
    """Simple deterministic unicycle model for safety and interface tests."""

    def __init__(self, pose: Pose = Pose()) -> None:
        self.pose = pose

    def step(self, command: Command, dt_s: float) -> Pose:
        if dt_s <= 0:
            raise ValueError("dt_s must be positive")
        yaw = self.pose.yaw_rad + command.angular_rps * dt_s
        x = self.pose.x_m + command.linear_mps * cos(self.pose.yaw_rad) * dt_s
        y = self.pose.y_m + command.linear_mps * sin(self.pose.yaw_rad) * dt_s
        self.pose = Pose(x, y, yaw)
        return self.pose
