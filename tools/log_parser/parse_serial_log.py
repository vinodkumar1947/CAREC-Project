#!/usr/bin/env python3
"""
parse_serial_log.py — CAREC serial log analyser

Parses the firmware serial output captured by firmware/tools/monitor.sh
and produces a human-readable summary: detection counts, zone distribution,
false positive rate, and average distance per zone.

Usage:
    python tools/log_parser/parse_serial_log.py logs/serial_20260504_093000.log

Output:
    - Zone distribution table
    - Average distance per zone
    - Estimated false positive rate (YELLOW/RED events while stationary)
    - BLE event list
"""

import argparse
import re
import sys
from collections import Counter, defaultdict

# ── Log line patterns ─────────────────────────────────────────────────────────
RE_SAFETY  = re.compile(r'\[Safety\] Nearest obstacle: ([\d.]+) cm\s+motion=(\w+)')
RE_ZONE    = re.compile(r'\[Safety\] Zone: (\w+)')
RE_BLE     = re.compile(r'\[BLE\] (\{.+\})')
RE_MOTION  = re.compile(r'\[Motion\]\s+(\w+)')

def parse_log(path):
    safety_events = []
    zone_events   = []
    ble_events    = []
    motion_events = []

    with open(path) as f:
        for line in f:
            line = line.strip()

            m = RE_SAFETY.search(line)
            if m:
                safety_events.append({"dist_cm": float(m.group(1)), "motion": m.group(2)})

            m = RE_ZONE.search(line)
            if m:
                zone_events.append(m.group(1))

            m = RE_BLE.search(line)
            if m:
                ble_events.append(m.group(1))

            m = RE_MOTION.search(line)
            if m:
                motion_events.append(m.group(1))

    return safety_events, zone_events, ble_events, motion_events


def main():
    parser = argparse.ArgumentParser(description="CAREC serial log analyser")
    parser.add_argument("logfile", help="Path to serial log file")
    args = parser.parse_args()

    safety, zones, ble, motions = parse_log(args.logfile)

    total_loops = len(zones)
    if total_loops == 0:
        print("No zone events found. Is this a CAREC serial log?")
        sys.exit(1)

    # ── Zone distribution ─────────────────────────────────────────────────────
    zone_counts = Counter(zones)
    print("=== CAREC Log Analysis ===")
    print(f"Log file     : {args.logfile}")
    print(f"Total loops  : {total_loops}")
    print()

    print("--- Zone Distribution ---")
    for zone in ["GREEN", "YELLOW", "RED"]:
        count = zone_counts.get(zone, 0)
        pct = 100 * count / total_loops
        bar = "█" * int(pct / 2)
        print(f"  {zone:<8} : {count:5d} ({pct:5.1f}%)  {bar}")
    print()

    # ── Average distance per zone ─────────────────────────────────────────────
    dist_by_zone = defaultdict(list)
    for ev in safety:
        motion = ev["motion"]
        dist   = ev["dist_cm"]
        if dist < 60:
            z = "RED"
        elif dist < 100:
            z = "YELLOW"
        else:
            z = "GREEN"
        dist_by_zone[z].append(dist)

    print("--- Average Distance per Zone ---")
    for zone in ["RED", "YELLOW", "GREEN"]:
        dists = dist_by_zone.get(zone, [])
        if dists:
            avg = sum(dists) / len(dists)
            mn  = min(dists)
            mx  = max(dists)
            print(f"  {zone:<8} : avg={avg:6.1f} cm  min={mn:5.1f}  max={mx:5.1f}  n={len(dists)}")
    print()

    # ── Motion distribution ───────────────────────────────────────────────────
    motion_counts = Counter(motions)
    print("--- Motion Distribution ---")
    for m in ["STATIONARY", "FWD", "BWD"]:
        count = motion_counts.get(m, 0)
        pct = 100 * count / max(len(motions), 1)
        print(f"  {m:<12} : {count:5d} ({pct:5.1f}%)")
    print()

    # ── BLE events ────────────────────────────────────────────────────────────
    print(f"--- BLE Events: {len(ble)} total ---")
    for ev in ble[-10:]:  # last 10
        print(f"  {ev}")
    if len(ble) > 10:
        print(f"  ... ({len(ble) - 10} more)")
    print()

    # ── Summary ───────────────────────────────────────────────────────────────
    non_green = zone_counts.get("YELLOW", 0) + zone_counts.get("RED", 0)
    alert_rate = 100 * non_green / total_loops
    print("--- Summary ---")
    print(f"  Alert rate (YELLOW+RED) : {alert_rate:.1f}%")
    print(f"  CRITICAL events (RED)   : {zone_counts.get('RED', 0)}")
    print(f"  BLE events sent         : {len(ble)}")


if __name__ == "__main__":
    main()
