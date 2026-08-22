# Abstract Drafts

**Target:** 150–200 words | Springer LNCS format

---

## Draft v0.1 (May 5, 2026)

Electric wheelchair users, particularly children, face significant risk of obstacle collisions due to limited spatial awareness and slower reaction times. Existing commercial safety solutions such as the Braze Mobility Blind Spot Sensor cost between $1,850 and $4,040, are rear-facing only, and lack wireless connectivity — placing them beyond reach for most families.

We present **CAREC** (Collision Avoidance with Real-time Edge Computing), an open-source, non-invasive obstacle detection system built on the SenseCAP Watcher W1-A — a single $59.99 device integrating a 5MP OV5647 camera, Himax HX6538 neural processing unit, 1.45" colour display, 1W speaker, and Bluetooth 5. CAREC uses on-device NPU inference to detect obstacles in real time and classifies them into three distance zones — critical (<60cm), warning (60–100cm), and clear (100+cm) — triggering synchronised audio, visual, and BLE caregiver alerts with end-to-end latency under 200ms. An optical-flow motion gate eliminates false positives when the wheelchair is stationary. The complete system costs under $100, requires no wheelchair modification, and achieved [XX]% detection accuracy across 50 obstacle scenarios.

**Keywords:** edge AI, obstacle detection, wheelchair safety, NPU inference, assistive technology, computer vision

---

## Notes for Revision

- Fill in accuracy percentage after Week 3–4 testing
- Confirm exact latency figure from `tools/log_parser/parse_serial_log.py` output
- Add one sentence on BLE caregiver app if word count allows
- Keep under 200 words — currently ~[count] words
- Springer LNCS: abstract should not contain citations or figure references
