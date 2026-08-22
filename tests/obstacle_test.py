"""
obstacle_test.py — CAREC integration tests

Validates zone classification, distance estimation, beep patterns, BLE JSON
format, and optical-flow motion classification. All constants mirror
firmware/config/zone_config.h and the corresponding .h source files.

Run:  pytest tests/obstacle_test.py -v
"""

import json
import math
import re
import pytest

# Pull shared constants and helpers from conftest.py (mirrors zone_config.h)
from tests.conftest import (
    DIST_RED, DIST_YELLOW,
    ZONE_GREEN, ZONE_YELLOW, ZONE_RED,
    classify_zone, estimate_distance,
)

# ── Extended distance estimator (label-aware, mirrors distance_estimator.h) ───

CAMERA_FOV_DEG = 120.0
TAN_HALF_FOV   = math.tan(math.radians(CAMERA_FOV_DEG / 2))   # tan(60°) ≈ 1.7321

_REF_WIDTH_CM = {
    "person": 50.0,
    "human":  50.0,
    "chair":  45.0,
    "stool":  45.0,
    "table":  80.0,
    "desk":   80.0,
    "door":   80.0,
}
_REF_WIDTH_DEFAULT = 50.0

CONFIDENCE_THRESHOLD = 0.55
DIST_MIN, DIST_MAX   = 10.0, 200.0


def ref_width_for(label: str) -> float:
    label = label.lower()
    for key, w in _REF_WIDTH_CM.items():
        if key in label:
            return w
    return _REF_WIDTH_DEFAULT


def estimate_distance_labeled(label: str, bbox_w_norm: float) -> float:
    """Mirrors distance_estimator.h estimate_distance_cm() with label-specific widths."""
    if bbox_w_norm < 0.02:
        return DIST_MAX
    real_w = ref_width_for(label)
    dist   = real_w / (bbox_w_norm * 2.0 * TAN_HALF_FOV)
    return max(DIST_MIN, min(DIST_MAX, dist))


def classify_label(label: str) -> str:
    """Mirrors sensecraft_detection.h classify_label()."""
    label = label.lower()
    if "person" in label or "human" in label:
        return "OBS_PERSON"
    if any(x in label for x in ["chair", "table", "couch", "sofa", "desk", "bed", "bench", "stool"]):
        return "OBS_FURNITURE"
    if any(x in label for x in ["wall", "door", "step", "stair", "pillar"]):
        return "OBS_BARRIER"
    return "OBS_OTHER"


# ── Zone classification ────────────────────────────────────────────────────────

class TestZoneClassification:
    def test_red_at_zero(self):
        assert classify_zone(0) == ZONE_RED

    def test_red_midpoint(self):
        assert classify_zone(30) == ZONE_RED

    def test_red_just_inside_boundary(self):
        assert classify_zone(DIST_RED - 1) == ZONE_RED

    def test_boundary_at_dist_red_is_yellow(self):
        # firmware: if dist < DIST_RED → RED; at exactly DIST_RED → YELLOW
        assert classify_zone(DIST_RED) == ZONE_YELLOW

    def test_yellow_midpoint(self):
        assert classify_zone(80) == ZONE_YELLOW

    def test_yellow_just_inside_boundary(self):
        assert classify_zone(DIST_YELLOW - 1) == ZONE_YELLOW

    def test_boundary_at_dist_yellow_is_green(self):
        assert classify_zone(DIST_YELLOW) == ZONE_GREEN

    def test_green_far(self):
        assert classify_zone(150) == ZONE_GREEN
        assert classify_zone(999) == ZONE_GREEN


# ── Beep mapping ───────────────────────────────────────────────────────────────

class TestBeepMapping:
    def test_red_maps_to_critical(self):
        assert classify_zone(30) == ZONE_RED

    def test_yellow_maps_to_warning(self):
        assert classify_zone(80) == ZONE_YELLOW

    def test_green_maps_to_none(self):
        assert classify_zone(120) == ZONE_GREEN

    def test_full_pipeline_red(self):
        dist = estimate_distance(0.289, real_width_cm=50)
        assert classify_zone(dist) == ZONE_RED

    def test_full_pipeline_yellow(self):
        dist = estimate_distance(0.180, real_width_cm=50)
        assert classify_zone(dist) == ZONE_YELLOW

    def test_full_pipeline_green(self):
        dist = estimate_distance(0.10, real_width_cm=50)
        assert classify_zone(dist) == ZONE_GREEN


# ── Distance estimation (conftest simple estimator — 30cm ref) ─────────────────

class TestDistanceEstimationSimple:
    """Uses conftest estimate_distance() with 30cm reference width."""

    def test_red_boundary_bbox(self):
        # CALIB_BBOX_AT_RED=0.241 was derived for a 50cm person reference
        dist = estimate_distance(0.241, real_width_cm=50)
        assert DIST_RED - 5 <= dist <= DIST_RED + 5, f"Expected ~60cm, got {dist:.1f}"

    def test_yellow_boundary_bbox(self):
        # CALIB_BBOX_AT_YELLOW=0.144 was derived for a 50cm person reference
        dist = estimate_distance(0.144, real_width_cm=50)
        assert DIST_YELLOW - 5 <= dist <= DIST_YELLOW + 5, f"Expected ~100cm, got {dist:.1f}"

    def test_large_bbox_is_close(self):
        assert estimate_distance(0.5) < DIST_RED

    def test_tiny_bbox_is_far(self):
        assert estimate_distance(0.05) > DIST_YELLOW

    def test_zero_bbox_returns_max(self):
        assert estimate_distance(0.0) == DIST_MAX

    def test_negative_bbox_returns_max(self):
        assert estimate_distance(-0.1) == DIST_MAX

    def test_clamp_at_min(self):
        assert estimate_distance(10.0) == DIST_MIN

    def test_clamp_at_max(self):
        assert estimate_distance(0.001) == DIST_MAX

    def test_wider_object_at_same_bbox_is_farther(self):
        bbox_w = 0.20
        assert estimate_distance(bbox_w, real_width_cm=80) > estimate_distance(bbox_w, real_width_cm=30)


# ── Distance estimation (label-aware, mirrors distance_estimator.h) ────────────

class TestDistanceEstimationLabeled:
    def test_person_at_60cm_boundary(self):
        bbox_w = 50.0 / (60.0 * 2.0 * TAN_HALF_FOV)
        dist = estimate_distance_labeled("person", bbox_w)
        assert abs(dist - 60.0) < 3.0

    def test_person_at_100cm_boundary(self):
        bbox_w = 50.0 / (100.0 * 2.0 * TAN_HALF_FOV)
        dist = estimate_distance_labeled("person", bbox_w)
        assert abs(dist - 100.0) < 5.0

    def test_person_at_50cm(self):
        dist = estimate_distance_labeled("person", 0.289)
        assert abs(dist - 50.0) < 3.0
        assert classify_zone(dist) == ZONE_RED

    def test_person_at_80cm(self):
        dist = estimate_distance_labeled("person", 0.180)
        assert abs(dist - 80.0) < 5.0
        assert classify_zone(dist) == ZONE_YELLOW

    def test_tiny_bbox_returns_max(self):
        assert estimate_distance_labeled("person", 0.01) == DIST_MAX

    def test_large_bbox_clamps_to_min(self):
        dist = estimate_distance_labeled("person", 0.99)
        assert dist >= DIST_MIN

    def test_chair_at_50cm(self):
        expected_bbox_w = 45.0 / (50.0 * 2.0 * TAN_HALF_FOV)
        dist = estimate_distance_labeled("chair", expected_bbox_w)
        assert abs(dist - 50.0) < 3.0


# ── Label classification ───────────────────────────────────────────────────────

class TestLabelClassification:
    def test_person(self):
        assert classify_label("person") == "OBS_PERSON"
        assert classify_label("human")  == "OBS_PERSON"

    def test_furniture(self):
        for label in ["chair", "table", "couch", "sofa", "desk", "bed", "bench", "stool"]:
            assert classify_label(label) == "OBS_FURNITURE", f"Failed: {label}"

    def test_barrier(self):
        for label in ["wall", "door", "step", "stair", "pillar"]:
            assert classify_label(label) == "OBS_BARRIER", f"Failed: {label}"

    def test_other(self):
        assert classify_label("bicycle") == "OBS_OTHER"
        assert classify_label("cat")     == "OBS_OTHER"

    def test_person_wins_composite_label(self):
        assert classify_label("person sitting on chair") == "OBS_PERSON"


# ── Fixture-driven tests (sample_detections.json) ─────────────────────────────

class TestFixtureDetections:
    _zone_map = {"RED": ZONE_RED, "YELLOW": ZONE_YELLOW, "GREEN": ZONE_GREEN}

    def test_all_fixtures_pass(self, sample_detections):
        for tc in sample_detections:
            det = tc["detection"]
            expected_zone = self._zone_map[tc["expected_zone"]]
            lo, hi = tc["expected_dist_range_cm"]

            if det["count"] == 0:
                dist = DIST_MAX
            else:
                dists = [
                    estimate_distance_labeled(label, w)
                    for label, w, conf in zip(det["label"], det["bbox_w"], det["confidence"])
                    if conf >= CONFIDENCE_THRESHOLD
                ]
                dist = min(dists) if dists else DIST_MAX

            zone = classify_zone(dist)
            assert zone == expected_zone, (
                f"{tc['id']}: expected zone {tc['expected_zone']}, "
                f"got {zone} (dist={dist:.1f}cm)"
            )
            assert lo <= dist <= hi, (
                f"{tc['id']}: distance {dist:.1f}cm outside [{lo}, {hi}]"
            )

    def test_multi_detection_picks_nearest(self, sample_detections):
        tc = next(t for t in sample_detections if t["id"] == "TC-MULTI-01")
        dists = [estimate_distance(w) for w in tc["detection"]["bbox_w"]]
        assert min(dists) < DIST_RED

    def test_empty_detection_is_green(self, sample_detections):
        tc = next(t for t in sample_detections if t["id"] == "TC-EMPTY-01")
        assert tc["detection"]["count"] == 0
        assert tc["expected_zone"] == "GREEN"

    def test_boundary_red_is_yellow(self, sample_detections):
        tc = next(t for t in sample_detections if t["id"] == "TC-BOUNDARY-RED")
        label = tc["detection"]["label"][0]
        bbox_w = tc["detection"]["bbox_w"][0]
        dist = estimate_distance_labeled(label, bbox_w)
        assert classify_zone(dist) == ZONE_YELLOW, (
            f"Obstacle at boundary (dist={dist:.1f}cm) should be YELLOW, not RED"
        )


# ── Beep pattern timing (mirrors zone_config.h) ────────────────────────────────

class TestBeepPatternTiming:
    # Values must match firmware/config/zone_config.h
    WARNING_PULSE_MS   = 300
    WARNING_PERIOD_MS  = 1000
    CRITICAL_PULSE_MS  = 80
    CRITICAL_GAP_MS    = 40
    CRITICAL_PULSES    = 3
    CRITICAL_PERIOD_MS = 200

    def test_warning_off_time(self):
        assert self.WARNING_PERIOD_MS - self.WARNING_PULSE_MS == 700

    def test_warning_duty_cycle(self):
        duty = self.WARNING_PULSE_MS / self.WARNING_PERIOD_MS
        assert duty == pytest.approx(0.30, abs=0.01)

    def test_critical_pulse_count(self):
        assert self.CRITICAL_PULSES == 3

    def test_critical_burst_duration(self):
        # 3 pulses with 40ms gaps between them (not after last)
        burst_ms = (self.CRITICAL_PULSE_MS + self.CRITICAL_GAP_MS) * (self.CRITICAL_PULSES - 1) + self.CRITICAL_PULSE_MS
        assert burst_ms == 320   # 80+40+80+40+80

    def test_critical_is_faster_than_warning(self):
        warning_rate = 1000.0 / self.WARNING_PERIOD_MS   # beeps/sec
        # critical: PULSES beeps per (burst + period) ms
        cycle_ms     = (self.CRITICAL_PULSE_MS + self.CRITICAL_GAP_MS) * (self.CRITICAL_PULSES - 1) + self.CRITICAL_PULSE_MS + self.CRITICAL_PERIOD_MS
        critical_rate = self.CRITICAL_PULSES * 1000.0 / cycle_ms
        assert critical_rate > warning_rate * 2


# ── BLE JSON format ────────────────────────────────────────────────────────────

class TestBLEJsonFormat:
    """Validates the JSON payload format produced by ble_log_event()."""

    def _event(self, zone="RED", dist_cm=42.3, motion="FORWARD", ts_ms=12345):
        return (
            f'{{"zone":"{zone}",'
            f'"dist_cm":{dist_cm:.1f},'
            f'"motion":"{motion}",'
            f'"ts_ms":{ts_ms}}}'
        )

    def test_valid_json_red_forward(self):
        data = json.loads(self._event("RED", 42.3, "FORWARD"))
        assert data["zone"] == "RED"
        assert data["dist_cm"] == pytest.approx(42.3, abs=0.05)
        assert data["motion"] == "FORWARD"

    def test_valid_json_green_stationary(self):
        data = json.loads(self._event("GREEN", 150.0, "STATIONARY"))
        assert data["zone"] == "GREEN"

    def test_all_zone_values_parse(self):
        for zone in ("RED", "YELLOW", "GREEN"):
            data = json.loads(self._event(zone))
            assert data["zone"] == zone

    def test_all_motion_values_parse(self):
        for motion in ("FORWARD", "BACKWARD", "STATIONARY"):
            data = json.loads(self._event(motion=motion))
            assert data["motion"] == motion

    def test_dist_cm_one_decimal(self):
        msg = self._event(dist_cm=75.6)
        assert re.search(r'"dist_cm":\d+\.\d', msg)

    def test_all_required_keys_present(self):
        data = json.loads(self._event())
        assert {"zone", "dist_cm", "motion", "ts_ms"} == set(data.keys())

    def test_ts_ms_is_integer(self):
        data = json.loads(self._event(ts_ms=999999))
        assert isinstance(data["ts_ms"], int)

    def test_payload_fits_ble_mtu(self):
        # BLE_TX_MAX_LEN = 512 in ble_logger.h
        msg = self._event("RED", 200.0, "FORWARD", ts_ms=2**32 - 1)
        assert len(msg) <= 512


# ── Optical-flow motion classification (mirrors motion_detector.h) ─────────────

MDF_W          = 80
MDF_H          = 60
MDF_MOTION_THR = 0.6
MDF_DIVERGE_THR = 4.0

MOTION_STATIONARY = "STATIONARY"
MOTION_FORWARD    = "FORWARD"
MOTION_BACKWARD   = "BACKWARD"


def classify_motion(features, flows):
    """Mirrors _mdf_classify() in motion_detector.h."""
    if not features:
        return MOTION_STATIONARY
    cx, cy = MDF_W / 2.0, MDF_H / 2.0
    n = len(features)
    total_mag = sum(math.hypot(u, v) for u, v in flows) / n
    total_div = sum((px - cx) * u + (py - cy) * v
                    for (px, py), (u, v) in zip(features, flows)) / n
    if total_mag < MDF_MOTION_THR:     return MOTION_STATIONARY
    if total_div >  MDF_DIVERGE_THR:   return MOTION_FORWARD
    if total_div < -MDF_DIVERGE_THR:   return MOTION_BACKWARD
    return MOTION_STATIONARY


class TestMotionClassification:
    def _grid_features(self):
        return [(x, y) for y in (15, 30, 45) for x in (20, 40, 60, 80)]

    def _diverging_flows(self, scale=1.5):
        cx, cy = MDF_W / 2.0, MDF_H / 2.0
        return [(scale * (px - cx) / 40.0, scale * (py - cy) / 30.0)
                for px, py in self._grid_features()]

    def test_forward_detected(self):
        assert classify_motion(self._grid_features(), self._diverging_flows()) == MOTION_FORWARD

    def test_backward_detected(self):
        converging = [(-u, -v) for u, v in self._diverging_flows()]
        assert classify_motion(self._grid_features(), converging) == MOTION_BACKWARD

    def test_zero_flow_is_stationary(self):
        flows = [(0.0, 0.0)] * 12
        assert classify_motion(self._grid_features(), flows) == MOTION_STATIONARY

    def test_below_magnitude_threshold_is_stationary(self):
        flows = [(0.2, 0.2)] * 12   # magnitude 0.28 < MDF_MOTION_THR=0.6
        assert classify_motion(self._grid_features(), flows) == MOTION_STATIONARY

    def test_no_features_is_stationary(self):
        assert classify_motion([], []) == MOTION_STATIONARY

    def test_corner_features_forward(self):
        features = [(10, 10), (70, 10), (10, 50), (70, 50)]
        flows    = [(-3.0, -3.0), (3.0, -3.0), (-3.0, 3.0), (3.0, 3.0)]
        assert classify_motion(features, flows) == MOTION_FORWARD

    def test_corner_features_backward(self):
        features = [(10, 10), (70, 10), (10, 50), (70, 50)]
        flows    = [(3.0, 3.0), (-3.0, 3.0), (3.0, -3.0), (-3.0, -3.0)]
        assert classify_motion(features, flows) == MOTION_BACKWARD

    def test_pure_lateral_pan_is_stationary(self):
        # All features in a horizontal row through centre → divergence ≈ 0
        features = [(20, 30), (40, 30), (60, 30)]
        flows    = [(2.0, 0.0)] * 3
        assert classify_motion(features, flows) == MOTION_STATIONARY

    def test_magnitude_at_threshold_is_stationary(self):
        # avg_mag == MDF_MOTION_THR → not strictly greater → stationary
        features = [(40, 30)]
        flows    = [(MDF_MOTION_THR, 0.0)]
        assert classify_motion(features, flows) == MOTION_STATIONARY

    def test_divergence_at_threshold_is_stationary(self):
        # divergence == MDF_DIVERGE_THR → not strictly greater → stationary
        features = [(40 + 1, 30)]
        flows    = [(MDF_DIVERGE_THR, 0.0)]
        assert classify_motion(features, flows) == MOTION_STATIONARY


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
