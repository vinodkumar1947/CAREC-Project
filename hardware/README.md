# Hardware — CAREC Wheelchair Safety System

**Device:** SenseCAP Watcher W1-A Clear Enclosure — The Physical AI Agent for Smarter Spaces  
**Mount:** Ball joint + tube clamp (non-invasive, no drilling)

---

## Mounting Design

```
SenseCAP Watcher W1-A
    ↓ 1/4" thread
Ball Joint (360° rotation + ±80° tilt)
    ↓
Tube Clamp (1–1.5" adjustable, stainless steel)
    ↓
Wheelchair Armrest (Numotion power wheelchair)
```

**Install:** 5 minutes | **Remove:** 2 minutes | **Damage:** Zero

---

## Assembly Steps

1. Thread ball joint onto SenseCAP Watcher (1/4" standard mount)
2. Attach tube clamp to ball joint stud
3. Measure armrest diameter (target: 1–1.5")
4. Open clamp, slide around armrest, tighten — finger-tight + 1/4 turn
5. Adjust camera angle — point forward at ~45° downward tilt
6. Plug in USB-C from external battery pack
7. Test: camera 120° FOV covers obstacle zone ahead of wheels

---

## Camera Placement

```
              FRONT OF WHEELCHAIR
    ┌──────────────────────────────┐
    │                              │
    │   SenseCAP Watcher W1-A      │ ← mounted on armrest
    │   120° FOV (forward-facing)  │
    │                              │
    └──────────────────────────────┘
         ↓ sees obstacle zone
    [obstacle]  [obstacle]  [obstacle]
```

- Camera is forward-facing (detects obstacles in path)
- 120° horizontal FOV covers full wheelchair width + margins
- Distance thresholds: 0–60cm RED (BEEP_CRITICAL), 60–100cm YELLOW (BEEP_WARNING), 100+cm GREEN (silent)

---

## Power

- External LiPo 10000mAh 3.7V via USB-C → 50+ hours runtime
- SenseCAP Watcher internal battery: ~6 hours (use external for daily use)
- Charging: USB-C (standard 5V)

---

## Expansion (Phase 2+)

The SenseCAP Watcher has Grove IIC + GPIO headers for adding:
- Additional ultrasonic sensors (rear obstacle detection)
- External buzzer (if internal 1W speaker isn't loud enough in noisy environments)

---

## Files (Coming Soon)

- `schematics/carec_mount_diagram.pdf` — mounting assembly diagram
- `3d-models/armrest_clamp_adapter.stl` — custom adapter if needed

## See Also

- [Bill of Materials](BOM.md) — what was ordered and costs
- [Hardware Status Update](../docs/specifications/HARDWARE_STATUS_UPDATE.md) — delivery timeline
