# CAREC Test Plan

**Version:** 0.3.0  
**Target completion:** Week 4 (before any unsupervised use)  
**Last updated:** May 19, 2026

This document defines the validation plan for CAREC before the v1.0 release and before any unsupervised child use. Tests are grouped into three gates: **build gate** (automated CI), **bench gate** (controlled environment), and **deployment gate** (real wheelchair, supervised).

---

## Pass Criteria Summary

| Metric | Target | Gate |
|--------|--------|------|
| Build | 0 errors, 0 warnings (CAREC sources) | Build |
| Alert latency | < 200ms (detection → audio) | Bench |
| Detection accuracy | ≥ 85% on standard obstacle matrix | Bench |
| False positive rate | ≤ 5% (stationary and low-contrast) | Bench |
| Battery runtime | ≥ 50 hours continuous | Bench |
| Caregiver app connectivity | Zone events visible ≤ 2s after detection | Bench |
| OTA update | Clean reboot to CAREC ready state | Bench |
| Wheelchair mounting | Zero movement when armrest is shaken firmly | Deployment |
| Supervised use | 30-minute session without missed CRITICAL events | Deployment |

---

## Gate 1 — Build Gate (Automated CI)

Run on every push to `main` and every pull request.

**Pass criteria:**
- `idf.py build` exits with code 0
- Zero errors and zero warnings in CAREC source files
- `python -m pytest tests/ -v` — all tests pass
- Binary size < 2.5 MB (leaves headroom for OTA partitions)

**CI configuration:** `.github/workflows/build.yml`

---

## Gate 2 — Bench Validation

Performed in a controlled indoor environment. Record results in `tests/results/bench_YYYY-MM-DD.md`.

### 2A — Alert Latency

**Method:**
1. Connect a second device to the serial monitor
2. Trigger a CRITICAL zone obstacle (hand at 30 cm)
3. Record timestamp of detection log line and timestamp of first beep
4. Repeat 10 times, record each result

**Pass criteria:** All 10 trials < 200ms from detection log to first beep.

---

### 2B — 50-Obstacle Detection Matrix

Test each combination of obstacle type and distance. Record Pass / Fail.

**Obstacle types (6):**

| ID | Obstacle |
|----|---------|
| OB-1 | Chair leg (thin, < 3 cm diameter) |
| OB-2 | Table leg (wide, > 5 cm diameter) |
| OB-3 | Wall (flat, full width) |
| OB-4 | Person (standing adult) |
| OB-5 | Person (crouching / child height) |
| OB-6 | Backpack or bag on floor |

**Distances (3):**

| Zone | Distance | Expected alert |
|------|----------|----------------|
| CRITICAL | 30 cm | RED display + 3× fast beeps |
| WARNING | 80 cm | YELLOW display + 1× slow beep |
| CLEAR | 130 cm | GREEN display + silence |

**Test matrix (18 cells × 3 repeats = 54 trials):**

| Obstacle | 30 cm (CRITICAL) | 80 cm (WARNING) | 130 cm (CLEAR) |
|---------|-----------------|----------------|---------------|
| OB-1 Chair leg | Pass/Fail | Pass/Fail | Pass/Fail |
| OB-2 Table leg | Pass/Fail | Pass/Fail | Pass/Fail |
| OB-3 Wall | Pass/Fail | Pass/Fail | Pass/Fail |
| OB-4 Person standing | Pass/Fail | Pass/Fail | Pass/Fail |
| OB-5 Person crouching | Pass/Fail | Pass/Fail | Pass/Fail |
| OB-6 Backpack | Pass/Fail | Pass/Fail | Pass/Fail |

**Pass criteria:** ≥ 85% of trials pass (≥ 46/54). Zero false negatives for CRITICAL zone (OB-4 and OB-5 at 30 cm must be 100%).

---

### 2C — False Positive Rate

**Purpose:** Verify the motion gate and confidence threshold prevent spurious alerts.

**Tests:**

| Scenario | Expected result |
|---------|----------------|
| Wheelchair stationary, cluttered room | CLEAR zone, no beeps |
| Wheelchair stationary, person walks past at 150 cm | CLEAR zone, no beeps |
| Low-light room (< 50 lux) with OB-3 at 130 cm | CLEAR zone, no beeps |
| Reflective floor, no obstacle | CLEAR zone, no beeps |
| Fast-moving obstacle passing at 80 cm | WARNING zone OR CLEAR — not CRITICAL |

**Pass criteria:** ≤ 5 false positives across all 5 scenarios × 3 repeats = 15 trials.

---

### 2D — Battery Runtime Test

**Method:**
1. Charge to 100%
2. Run CAREC in normal detection mode (simulated wheelchair movement — obstacle presented every 30 seconds)
3. Record time to first alert failure or power-off
4. Log battery indicator readings every 2 hours

**Pass criteria:** ≥ 50 hours of continuous operation.

---

### 2E — Mobile App Connectivity

**Method:**
1. Pair SenseCraft Mate app to CAREC via Bluetooth
2. Trigger CRITICAL zone (30 cm obstacle)
3. Measure time from beep to zone event appearing in app log

**Pass criteria:** Zone event visible in app ≤ 2 seconds after audio alert. No missed events over a 10-trigger sequence.

---

### 2F — OTA Firmware Update

**Method:**
1. Connect CAREC to WiFi via app
2. Trigger an OTA update (use a known previous firmware version to update from)
3. Observe update progress in app
4. After reboot, verify firmware version updated and "CAREC ready." appears in serial log

**Pass criteria:** Update completes cleanly. Device returns to CAREC ready state. No data loss in device configuration.

---

## Gate 3 — Deployment Validation

Performed on the actual wheelchair with a supervising adult present. Do not proceed to unsupervised child use until Gate 3 passes.

### 3A — Physical Mounting

**Method:**
1. Mount CAREC to wheelchair armrest using tube clamp and ball joint
2. Camera aimed directly forward at approximately child eye height
3. Shake armrest firmly — CAREC should not shift or wobble
4. Check USB-C cable is routed away from wheels and moving parts

**Pass criteria:** No movement during armrest shake test. Cable fully clear of all moving parts.

---

### 3B — Field of View Verification

**Method:**
1. With CAREC mounted on wheelchair, place OB-3 (wall) directly ahead at 60 cm
2. Slowly rotate wheelchair 15° left and right from centre
3. Verify CRITICAL alert fires at all three positions

**Pass criteria:** CRITICAL alert fires within 200ms at centre and ±15° rotation.

---

### 3C — Audio Audibility

**Method:**
1. Trigger CRITICAL zone in a typical indoor room (kitchen/living room noise level)
2. Caregiver stands 3 metres away
3. Caregiver must be able to clearly identify CRITICAL vs WARNING beep pattern

**Pass criteria:** Caregiver correctly identifies CRITICAL and WARNING patterns on 5/5 trials.

---

### 3D — Supervised Use Session

**Method:**
1. Child uses wheelchair normally in home environment for 30 minutes
2. Caregiver introduces known obstacles at least 5 times during the session
3. Record: obstacle type, zone triggered, alert fired (Y/N), child reaction, false positives

**Pass criteria:**
- All 5 introduced obstacles trigger the correct zone
- No CRITICAL false positives during normal navigation
- Child reacts appropriately to WARNING/CRITICAL alerts (slows or stops)

---

## Test Result Logging

Record all Gate 2 results in:

```
tests/results/bench_YYYY-MM-DD.md
```

Use this template:

```markdown
# Bench Test Results — YYYY-MM-DD

**Firmware version:** vX.Y.Z
**Tester:** [name]
**Environment:** [room type, lighting conditions]

## 2A — Alert Latency
| Trial | Latency (ms) | Pass |
|-------|-------------|------|
| 1     |             |      |
...

## 2B — Obstacle Matrix
[table]

## Summary
Pass / Fail  
Notes: [any anomalies or observations]
```

---

## Known Limitations (as of Week 3)

| Limitation | Impact | Mitigation |
|-----------|--------|-----------|
| Monocular camera — no depth data | Distance estimation is heuristic, not measured | Calibrate thresholds against physical tests; document accuracy |
| SPD2010 LCD deferred (hardware suspect) | Visual feedback via RGB LED only | RGB LED is clearly visible; LCD fix is in roadmap |
| Outdoor environments not validated | Bright sunlight may reduce detection accuracy | Out-of-scope for v1.0; documented in ROADMAP.md |
| Static obstacles only in initial matrix | Slow-moving obstacles may behave differently | Add moving-obstacle tests in v1.1 |
