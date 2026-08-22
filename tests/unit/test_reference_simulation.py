import pytest

from autonomy_ws.carec_simulation import Command, SafetyInput, SafetySupervisor, WheelchairSim


def state(**changes):
    values = dict(
        now_ms=1_000,
        command=Command(0.4, 0.0, 1_000),
        nearest_obstacle_m=2.0,
        sensor_stamp_ms=1_000,
        localization_valid=True,
        emergency_stop=False,
    )
    values.update(changes)
    return SafetyInput(**values)


@pytest.mark.parametrize(
    ("fault", "reason"),
    [
        ({"command": Command(0.4, 0.0, 700)}, "COMMAND_STALE"),
        ({"sensor_stamp_ms": 700}, "SENSOR_STALE"),
        ({"localization_valid": False}, "LOCALIZATION_INVALID"),
        ({"nearest_obstacle_m": 0.4}, "OBSTACLE_STOP"),
    ],
)
def test_faults_deterministically_stop(fault, reason):
    output = SafetySupervisor().evaluate(state(**fault))
    assert not output.allowed
    assert output.reason == reason
    assert output.command.linear_mps == 0.0
    assert output.command.angular_rps == 0.0


def test_emergency_stop_latches_until_authorized_reset():
    supervisor = SafetySupervisor()
    assert supervisor.evaluate(state(emergency_stop=True)).reason == "ESTOP_LATCHED"
    assert supervisor.evaluate(state()).reason == "ESTOP_LATCHED"
    assert not supervisor.reset_emergency_stop(physically_authorized=False)
    assert supervisor.reset_emergency_stop(physically_authorized=True)
    assert supervisor.evaluate(state()).reason == "OK"


def test_commands_are_limited():
    output = SafetySupervisor().evaluate(state(command=Command(2.0, -3.0, 1_000)))
    assert output.allowed
    assert output.reason == "LIMIT_APPLIED"
    assert output.command.linear_mps == pytest.approx(0.6)
    assert output.command.angular_rps == pytest.approx(-1.0)


def test_reverse_motion_is_not_blocked_by_forward_obstacle():
    output = SafetySupervisor().evaluate(
        state(command=Command(-0.2, 0.0, 1_000), nearest_obstacle_m=0.2)
    )
    assert output.allowed


def test_simulation_is_repeatable():
    first, second = WheelchairSim(), WheelchairSim()
    command = Command(0.5, 0.2, 0)
    for _ in range(20):
        first.step(command, 0.05)
        second.step(command, 0.05)
    assert first.pose == second.pose


def test_simulator_rejects_invalid_timestep():
    with pytest.raises(ValueError):
        WheelchairSim().step(Command(0.0, 0.0, 0), 0.0)
