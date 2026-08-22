"""
conftest.py — pytest configuration for CAREC tests

Provides shared fixtures and path setup for all test modules.
"""

import json
import math
import os
import sys
import pytest

# ── Path setup ────────────────────────────────────────────────────────────────
# Allow imports from project root
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, PROJECT_ROOT)

FIXTURES_DIR = os.path.join(os.path.dirname(__file__), "fixtures")

# ── Firmware constants (must match firmware/config/zone_config.h) ─────────────
DIST_RED    = 60   # cm — RED/YELLOW boundary
DIST_YELLOW = 100  # cm — YELLOW/GREEN boundary

ZONE_GREEN  = 0
ZONE_YELLOW = 1
ZONE_RED    = 2

DETECTION_OK = 0
DETECTION_NOT_READY = 1
DETECTION_INFERENCE_FAILED = 2

# ── Shared fixtures ───────────────────────────────────────────────────────────

@pytest.fixture(scope="session")
def sample_detections():
    """Load sample detection fixtures from fixtures/sample_detections.json."""
    path = os.path.join(FIXTURES_DIR, "sample_detections.json")
    with open(path) as f:
        data = json.load(f)
    return data["test_cases"]


@pytest.fixture(scope="session")
def zone_thresholds():
    """Return the current zone threshold constants."""
    return {"dist_red": DIST_RED, "dist_yellow": DIST_YELLOW}


def classify_zone(distance_cm):
    """Python equivalent of the firmware zone classification logic."""
    if distance_cm < DIST_RED:
        return ZONE_RED
    elif distance_cm < DIST_YELLOW:
        return ZONE_YELLOW
    else:
        return ZONE_GREEN


def classify_detection_result(status, distance_cm):
    """Mirror the firmware fail-safe boundary for detector health."""
    if status != DETECTION_OK:
        return ZONE_RED
    return classify_zone(distance_cm)


def estimate_distance(bbox_w_norm, real_width_cm=30.0, half_fov_deg=60.0):
    """
    Python equivalent of distance_estimator.h nearest_obstacle_cm().
    Formula: distance_cm = real_width_cm / (bbox_w_norm * 2 * tan(half_fov_deg))
    """
    if bbox_w_norm <= 0:
        return 200.0  # DIST_MAX_CM
    half_fov_rad = math.radians(half_fov_deg)
    distance = real_width_cm / (bbox_w_norm * 2.0 * math.tan(half_fov_rad))
    return max(10.0, min(200.0, distance))  # clamp to [DIST_MIN_CM, DIST_MAX_CM]


# Make helpers available as fixtures too
@pytest.fixture
def classify():
    return classify_zone


@pytest.fixture
def estimate():
    return estimate_distance
