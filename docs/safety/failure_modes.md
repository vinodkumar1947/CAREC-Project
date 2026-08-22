# CAREC Failure Mode Analysis

**Device:** SenseCAP Watcher W1-A  
**Standard:** Informal FMEA (Failure Mode and Effects Analysis)

---

## Risk Matrix

| Severity | Probability | Risk Level |
|----------|-------------|------------|
| HIGH (injury possible) | Any | CRITICAL — must mitigate |
| MEDIUM (nuisance/false alarm) | High | HIGH |
| LOW (feature degraded) | Any | LOW |

---

## Failure Modes

### FM-01: No Alert When Obstacle Present (False Negative)

| Field | Detail |
|-------|--------|
| **Failure Mode** | Obstacle present but no RED/YELLOW alert triggered |
| **Effect** | Wheelchair collides with obstacle |
| **Severity** | HIGH |
| **Causes** | Camera lens obscured; detection model misses small/low objects; CAREC powered off; firmware frozen |
| **Detection** | Pre-use 30 cm hand test; periodic spot checks |
| **Mitigation** | 1. Clear lens pre-use checklist. 2. `tests/obstacle_test.py` 50-scenario matrix. 3. CAREC is a *warning aid*, not a safety stop — caregiver supervision required. 4. Watchdog timer (future) to detect firmware freeze. |
| **Residual Risk** | MEDIUM — caregiver remains primary safety gate |

---

### FM-02: Continuous False Alarm (False Positive)

| Field | Detail |
|-------|--------|
| **Failure Mode** | Constant RED/beeping with no obstacle |
| **Effect** | Alert fatigue, child distress, caregiver ignores future real alerts |
| **Severity** | MEDIUM |
| **Causes** | Shadow/reflection triggering NPU; motion gate failure; firmware bug |
| **Detection** | 30-minute empty-room false positive test |
| **Mitigation** | 1. Motion gate: suppress all alerts when wheelchair stationary. 2. Rate-limit BLE events. 3. `DEBUG_VERBOSE` logging to identify pattern. |
| **Residual Risk** | LOW — motion gate eliminates most static false positives |

---

### FM-03: Device Falls Off Wheelchair

| Field | Detail |
|-------|--------|
| **Failure Mode** | Tube clamp loosens; Watcher detaches during use |
| **Effect** | Watcher hits ground; USB-C cable tangle risk; loss of safety coverage |
| **Severity** | HIGH |
| **Causes** | Insufficient clamp torque; vibration over time; child interference |
| **Detection** | Pre-use shake test; visual inspection |
| **Mitigation** | 1. Stainless tube clamp rated for 5 kg+. 2. Pre-use shake test (item in safety_checklist.md). 3. Weekly re-tightening schedule. |
| **Residual Risk** | LOW |

---

### FM-04: USB-C Cable Caught in Wheel

| Field | Detail |
|-------|--------|
| **Failure Mode** | Loose cable wraps around wheel axle |
| **Effect** | Wheelchair stops suddenly; cable damage; potential fall |
| **Severity** | HIGH |
| **Causes** | Poor cable routing; cable too long; velcro tie failure |
| **Detection** | Visual inspection during mounting; post-ride check |
| **Mitigation** | 1. Use shortest cable that reaches battery. 2. Velcro cable ties every 20 cm. 3. Route cable on armrest interior side. |
| **Residual Risk** | LOW |

---

### FM-05: Battery Depletion During Use

| Field | Detail |
|-------|--------|
| **Failure Mode** | 10,000 mAh battery runs out |
| **Effect** | CAREC powers off; no obstacle detection |
| **Severity** | MEDIUM |
| **Causes** | Battery not charged before session; higher than expected draw |
| **Detection** | Battery LED indicator; SenseCraft Mate battery status |
| **Mitigation** | 1. Pre-use battery check (≥ 20%). 2. 10,000 mAh → 50+ hr runtime. 3. Low-battery BLE alert (future feature). |
| **Residual Risk** | LOW |

---

### FM-06: OTA Update During Active Use

| Field | Detail |
|-------|--------|
| **Failure Mode** | Firmware update starts while wheelchair is moving |
| **Effect** | Device reboots mid-use; brief loss of obstacle detection |
| **Severity** | HIGH |
| **Causes** | OTA safety gate bypassed; WiFi reconnects in active zone |
| **Detection** | Code review of `ota_set_safe()` logic |
| **Mitigation** | 1. `ota_set_safe(zone == ZONE_GREEN)` — OTA only when GREEN. 2. `ota_set_safe(true)` only in STATIONARY branch. 3. `ota_check_and_update()` checks flag before downloading. |
| **Residual Risk** | LOW — double-gated (zone + motion) |

---

### FM-07: BLE/WiFi Interference

| Field | Detail |
|-------|--------|
| **Failure Mode** | BLE or WiFi connection drops |
| **Effect** | Caregiver app loses live updates |
| **Severity** | LOW (on-device alerts still work) |
| **Causes** | 2.4 GHz congestion; distance > 30 m BLE range |
| **Detection** | SenseCraft Mate connection indicator |
| **Mitigation** | 1. On-device beep+display works independently of BLE/WiFi. 2. Reconnect logic in `wifi_ota.h`. 3. BLE events queued and retried (future). |
| **Residual Risk** | LOW |

---

## Summary Risk Register

| ID | Failure Mode | Severity | Mitigated Risk |
|----|-------------|----------|---------------|
| FM-01 | False negative (miss obstacle) | HIGH | MEDIUM (caregiver backup) |
| FM-02 | False positive (constant alarm) | MEDIUM | LOW |
| FM-03 | Device falls off | HIGH | LOW |
| FM-04 | Cable in wheel | HIGH | LOW |
| FM-05 | Battery depletion | MEDIUM | LOW |
| FM-06 | OTA during use | HIGH | LOW |
| FM-07 | BLE/WiFi drop | LOW | LOW |

**Overall safety posture:** CAREC is a *warning aid*, not an autonomous safety system. FM-01 residual risk is acceptable only with caregiver supervision. Never use CAREC as a substitute for caregiver attention.

---

*Last Updated: May 4, 2026*
