# CAREC Safety Checklist

**Device:** SenseCAP Watcher W1-A Clear Enclosure  
**User:** 6-year-old child in electric wheelchair  
**Reviewer:** Caregiver + technical lead

---

## PRE-USE CHECKLIST (run before every session)

### Hardware Inspection

- [ ] Clear enclosure is unobstructed — no tape, stickers, dust, or scratches on camera lens
- [ ] Tube clamp is tight — Watcher does not shift when armrest is shaken firmly
- [ ] USB-C cable is fully seated at both ends (Watcher + battery)
- [ ] USB-C cable is routed clear of wheels and moving parts
- [ ] 10,000 mAh battery has ≥ 20% charge (check LED indicator)
- [ ] Battery is secured — cannot be removed by child during use

### Firmware / Connectivity

- [ ] CAREC boots to GREEN display within 10 seconds of power-on
- [ ] Serial monitor (if connected) shows "CAREC ready." at startup
- [ ] BLE event visible in SenseCraft Mate app within 30 seconds
- [ ] Place hand at 30 cm → display turns RED, critical beep sounds within 200ms
- [ ] Remove hand → display returns GREEN within 200ms

### Audio Test

- [ ] 1W speaker audible at 2 m with indoor ambient noise
- [ ] BEEP_WARNING (slow single beep) recognisable — test at 80 cm obstacle
- [ ] BEEP_CRITICAL (3× fast beeps) recognisable — test at 30 cm obstacle
- [ ] No speaker distortion or rattling at full volume

---

## INITIAL DEPLOYMENT CHECKLIST (first use only)

### 50-Obstacle Scenario Test Matrix

Complete before any unsupervised use. Log results in `tests/results/`.

| Obstacle Type | 30 cm (RED) | 80 cm (YELLOW) | 120 cm (GREEN) |
|--------------|-------------|----------------|----------------|
| Chair leg (thin) | Pass/Fail | Pass/Fail | Pass/Fail |
| Table leg (wide) | Pass/Fail | Pass/Fail | Pass/Fail |
| Wall (flat) | Pass/Fail | Pass/Fail | Pass/Fail |
| Person (standing) | Pass/Fail | Pass/Fail | Pass/Fail |
| Person (crouching) | Pass/Fail | Pass/Fail | Pass/Fail |
| Backpack on floor | Pass/Fail | Pass/Fail | Pass/Fail |
| Door frame | Pass/Fail | Pass/Fail | Pass/Fail |
| Pet / dog | Pass/Fail | Pass/Fail | Pass/Fail |
| Step / threshold | Pass/Fail | Pass/Fail | Pass/Fail |

**Target:** ≥ 85% detection accuracy, ≤ 5% false positive rate.

### 30-Minute False Positive Test

- [ ] Run CAREC in empty room for 30 minutes while wheelchair is stationary
- [ ] Motion gate must suppress all alerts (display stays GREEN)
- [ ] Count false positives: target < 3 events in 30 minutes

### Battery Life Test

- [ ] Run full session test: continuous operation for 8+ hours
- [ ] Verify no low-battery shutdown before 8 hours

### Caregiver Sign-Off

- [ ] Caregiver can identify all 3 display colours and their meanings
- [ ] Caregiver can identify BEEP_WARNING vs BEEP_CRITICAL by sound
- [ ] Caregiver knows how to power off (hold button 3 seconds)
- [ ] Caregiver can remove Watcher from wheelchair in under 2 minutes (emergency)
- [ ] Caregiver has SenseCraft Mate app installed + paired
- [ ] Caregiver knows CAREC does NOT replace supervision

**Caregiver sign-off:** _________________ Date: ___________

---

## ZONE DEFINITIONS (caregiver reference card)

| Display | Beep | Meaning | Action |
|---------|------|---------|--------|
| GREEN (solid) | None | No obstacles within 1m | Normal operation |
| YELLOW (slow blink) | Single beep | Obstacle 60–100 cm ahead | Caution — slow down |
| RED (fast blink) | Triple fast beep | Obstacle < 60 cm — imminent | STOP immediately |

---

## EMERGENCY PROCEDURES

| Situation | Action |
|-----------|--------|
| CAREC shows RED continuously with no obstacle | Power off, investigate (possible firmware freeze) |
| No beep when obstacle placed at 30 cm | Check speaker, check battery level |
| Watcher falls off wheelchair | Stop wheelchair, retrieve and reattach before continuing |
| USB-C cable caught in wheel | STOP immediately, remove cable from wheel |
| Child tries to remove device | Reposition cable routing, check clamp tightness |

---

*Last Updated: May 4, 2026*
