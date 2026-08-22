#!/usr/bin/env python3
"""
calibrate_distance.py — CAREC distance estimator calibration tool

Reads bounding-box data from Serial output while you hold a reference object
(e.g. 30 cm wide chair leg) at known distances. Computes the correct
CALIB_BBOX_AT_RED (60 cm) and CALIB_BBOX_AT_YELLOW (100 cm) constants
to paste into firmware/config/zone_config.h.

Usage:
    python tools/calibration/calibrate_distance.py --port /dev/cu.usbmodemXXXX

Instructions:
    1. Flash firmware with DEBUG_BBOX_RAW=1 in firmware/config/build_flags.h
    2. Run this script
    3. When prompted, hold the 30 cm reference object at each distance
    4. Copy the output constants into zone_config.h

Dependencies: pyserial
    pip install pyserial
"""

import argparse
import math
import re
import sys
import time

try:
    import serial
except ImportError:
    print("ERROR: pyserial not installed. Run: pip install pyserial")
    sys.exit(1)

# ── Configuration ─────────────────────────────────────────────────────────────
BAUD_RATE       = 115200
SAMPLE_COUNT    = 10         # samples to average at each distance
REF_WIDTH_CM    = 30.0       # width of your calibration object (cm)
HALF_FOV_DEG    = 60.0       # OV5647 half-FOV (120° total / 2)

CALIBRATION_DISTANCES = [60.0, 100.0]  # DIST_RED, DIST_YELLOW boundaries

# ── Regex to parse [BBOX] lines from firmware ─────────────────────────────────
# Expected format: [BBOX] x=0.35 y=0.40 w=0.241 h=0.30 conf=0.91 label=chair
BBOX_RE = re.compile(r'\[BBOX\].*w=([\d.]+)')

# ── Helpers ───────────────────────────────────────────────────────────────────

def estimate_distance(bbox_w_norm):
    """Compute distance estimate from bbox width."""
    if bbox_w_norm <= 0:
        return float('inf')
    half_fov_rad = math.radians(HALF_FOV_DEG)
    return REF_WIDTH_CM / (bbox_w_norm * 2.0 * math.tan(half_fov_rad))


def collect_samples(ser, count, timeout_s=30):
    """Read `count` bbox_w values from serial stream."""
    samples = []
    deadline = time.time() + timeout_s
    print(f"  Collecting {count} samples (timeout {timeout_s}s)... ", end="", flush=True)

    while len(samples) < count and time.time() < deadline:
        line = ser.readline().decode("utf-8", errors="replace").strip()
        m = BBOX_RE.search(line)
        if m:
            w = float(m.group(1))
            samples.append(w)
            print(".", end="", flush=True)

    print()
    return samples

# ── Main ──────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(description="CAREC distance calibration tool")
    parser.add_argument("--port", required=True, help="Serial port (e.g. /dev/cu.usbmodemXXXX)")
    parser.add_argument("--ref-width", type=float, default=REF_WIDTH_CM,
                        help=f"Reference object width in cm (default: {REF_WIDTH_CM})")
    args = parser.parse_args()

    print("=== CAREC Distance Calibration Tool ===")
    print(f"Port          : {args.port}")
    print(f"Reference obj : {args.ref_width} cm wide")
    print(f"Calibration at: {CALIBRATION_DISTANCES} cm")
    print()

    results = {}

    try:
        with serial.Serial(args.port, BAUD_RATE, timeout=1) as ser:
            for dist_cm in CALIBRATION_DISTANCES:
                print(f"--- Distance: {dist_cm:.0f} cm ---")
                input(f"  Hold the {args.ref_width} cm object exactly {dist_cm:.0f} cm from the camera.\n"
                      f"  Press ENTER when ready...")

                samples = collect_samples(ser, SAMPLE_COUNT)

                if len(samples) < 3:
                    print(f"  WARNING: Only {len(samples)} samples collected. "
                          "Check DEBUG_BBOX_RAW=1 in build_flags.h and retry.")
                    continue

                mean_w = sum(samples) / len(samples)
                estimated_dist = estimate_distance(mean_w)
                error_cm = abs(estimated_dist - dist_cm)

                print(f"  Samples     : {len(samples)}")
                print(f"  Mean bbox_w : {mean_w:.4f}")
                print(f"  Estimated   : {estimated_dist:.1f} cm  (error: {error_cm:.1f} cm)")
                results[dist_cm] = mean_w

    except serial.SerialException as e:
        print(f"Serial error: {e}")
        sys.exit(1)

    # ── Output ────────────────────────────────────────────────────────────────
    print()
    print("=== Calibration Results ===")
    print("Paste these values into firmware/config/zone_config.h:")
    print()

    if 60.0 in results:
        print(f"#define CALIB_BBOX_AT_RED     {results[60.0]:.4f}f  // bbox_w at 60 cm (DIST_RED)")
    if 100.0 in results:
        print(f"#define CALIB_BBOX_AT_YELLOW  {results[100.0]:.4f}f  // bbox_w at 100 cm (DIST_YELLOW)")


if __name__ == "__main__":
    main()
