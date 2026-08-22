# ISICVA 2026 — Paper Submission
## 2nd International Symposium on Innovations in Computer Vision and Applications

> **Status:** 🟡 In Preparation  
> **Submission Deadline: June 15, 2026** — 41 days away  
> **Conference: October 13–14, 2026** — Kolkata, India (Hybrid)  
> **Publication: Springer Nature LNNS Series (Scopus Indexed)**

---

## Conference Details

| Field | Detail |
|-------|--------|
| Full name | 2nd International Symposium on Innovations in Computer Vision and Applications |
| Short name | ISICVA 2026 |
| Organiser | Dept. of Computer Science & Engineering, Techno International New Town, Kolkata |
| Mode | Hybrid (in-person + online) |
| Publication | Springer Nature — Lecture Notes in Networks and Systems (LNNS) — Scopus Indexed* |
| Website | https://tint.edu.in/isicva2026 |
| Submission portal | https://cmt3.research.microsoft.com/ISICVA2026 |
| Contact | Dr. Chinmoy Kar — chinmoy.kar@tint.edu.in |
|         | Dr. Bitan Misra — bitan.misra@tint.edu.in |

*Approval pending

---

## Key Dates

| Milestone | Date | Days from Today (May 5) | Status |
|-----------|------|------------------------|--------|
| **Full Paper Submission** | **June 15, 2026** | **41 days** | ⬜ |
| Notification of Acceptance | July 15, 2026 | 71 days | ⬜ |
| Camera-Ready (CRC) Submission | July 31, 2026 | 87 days | ⬜ |
| Registration Deadline | August 15, 2026 | 102 days | ⬜ |
| Symposium | October 13–14, 2026 | 161 days | ⬜ |

---

## Registration Fees

| Category | Fee |
|----------|-----|
| Students | INR 7,000 |
| Academicians | INR 8,000 |
| Industry Professionals | INR 9,000 |
| National Participants | INR 3,000 |
| **International Participants** | **USD 150** |

---

## Our Paper

### Proposed Title
**"CAREC: A Real-Time Edge AI Obstacle Detection System for Pediatric Electric Wheelchairs Using On-Device Computer Vision"**

*(Alternative titles in `paper/outline.md`)*

### Target Track
**Track 4: Smart Vision Applications**
- ✅ Assistive technologies and accessibility solutions
- ✅ Smart healthcare and diagnostic systems
- ✅ Human-computer interaction and adaptive interfaces

**Also relevant to:**
- Track 1: Object Detection and Feature Extraction
- Track 3: Autonomous vehicles and robotic systems

### Paper Type
Full Paper — 8–12 pages (Springer LNCS format)

### Core Contribution
CAREC is the first documented open-source, sub-$100, plug-and-play AI obstacle detection system for pediatric electric wheelchairs, demonstrating:
1. On-device real-time inference (Himax HX6538 NPU, <150ms latency) — no cloud dependency
2. 3-tier distance classification (RED <60cm / YELLOW 60–100cm / GREEN 100+cm) via bounding-box heuristic
3. Multi-modal caregiver alerts (audio beep + color display + BLE push notification)
4. Optical-flow motion gate to eliminate false positives when stationary
5. Non-invasive mounting — zero wheelchair modification
6. OTA firmware update pipeline with safety-gated deployment

---

## Folder Structure

```
research/ISICVA2026/
├── README.md                ← This file — overview + submission tracker
│
├── paper/
│   ├── outline.md           ← Full paper structure, section-by-section plan
│   ├── abstract.md          ← Abstract drafts (target: 150–200 words)
│   ├── draft/               ← Working paper drafts (.md and .tex)
│   │   └── v0.1_draft.md    ← First full draft
│   ├── figures/             ← System diagrams, results charts, photos
│   │   ├── system_architecture.drawio
│   │   ├── detection_pipeline.png
│   │   └── zone_diagram.png
│   ├── data/                ← Raw test results, accuracy tables, CSV logs
│   │   ├── obstacle_test_results.csv
│   │   └── latency_measurements.csv
│   └── final/               ← Camera-ready PDF + source for CRC submission
│
├── submission/
│   └── checklist.md         ← Springer format requirements + submission steps
│
├── references/              ← BibTeX + annotated reference list
│   └── references.bib
│
└── reviews/                 ← Reviewer comments + response letters (post-acceptance)
```

---

## Writing Timeline (41 days to deadline)

| Week | Dates | Goal | Deliverable |
|------|-------|------|-------------|
| **Now → May 10** | May 5–10 | Hardware validation + data collection starts | Test results CSV started |
| **Week 1** | May 11–17 | Firmware working + outline finalised | `paper/outline.md` complete |
| **Week 2** | May 18–24 | Section 1–3 drafted (Intro, Related Work, System) | `draft/v0.1_draft.md` |
| **Week 3** | May 25–31 | Section 4–5 drafted (Results, Discussion) | `draft/v0.2_draft.md` |
| **Week 4** | Jun 1–7 | Full draft complete + figures done | `draft/v0.3_draft.md` |
| **Week 5** | Jun 8–14 | Proofread, format to Springer LNCS, final check | `final/CAREC_ISICVA2026.pdf` |
| **⬛ SUBMIT** | **Jun 15** | Submit via CMT3 portal | Confirmation email |

---

## Paper Sections Plan (summary)

| # | Section | Key content |
|---|---------|-------------|
| 1 | Introduction | Problem (pediatric wheelchair safety), gap ($4000 commercial alternatives), CAREC contribution |
| 2 | Related Work | Braze Mobility, academic AI wheelchair research, edge AI assistive tech |
| 3 | System Architecture | SenseCAP Watcher W1-A, NPU pipeline, zone logic, BLE, OTA |
| 4 | Implementation | Firmware modules, distance estimator, motion gate, non-blocking state machines |
| 5 | Experimental Results | Detection accuracy, latency, false positive rate, battery life |
| 6 | Discussion | Limitations (monocular, indoor-only), future work (rear sensor, LLM) |
| 7 | Conclusion | CAREC as open-source blueprint for affordable assistive AI |

Full section-by-section breakdown → [`paper/outline.md`](paper/outline.md)

---

## Success Criteria for Submission

- [ ] Detection accuracy ≥ 85% (50-obstacle test matrix)
- [ ] Latency ≤ 200ms (camera frame → first beep)
- [ ] False positive rate ≤ 5% (30-min stationary test)
- [ ] Battery runtime ≥ 8 hours documented
- [ ] System cost documented: ~$93–112 vs $1,850–4,040 commercial alternative
- [ ] Full paper ≥ 8 pages in Springer LNCS format
- [ ] All figures original (no reproduced copyrighted images)
- [ ] Abstract ≤ 200 words
- [ ] References ≥ 15 (IEEE/ACM/Springer sources)
