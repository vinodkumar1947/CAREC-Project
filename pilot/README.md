# CAREC accessibility review pilot

A buildless browser-local contributor starter. Open `index.html`, or from the repository root run:

```bash
python3 -m http.server 8000 --bind 127.0.0.1 --directory pilot
```

Open `http://127.0.0.1:8000`. Node.js 22+ is used only for tests:

```bash
node --test tests/pilot/review.test.cjs
```

## Capabilities and boundaries

- Illustrated sample with three authored, hypothetical timeline observations.
- Local video playback and manual timestamped notes.
- Confirm/dismiss/reset review decisions.
- JSON export and browser print/PDF.
- In-memory state only; export before refresh or source switch.

No automatic detection, inference model, calibrated geometry, safe-route determination, cloud upload, or physical control. Browser video codecs and print output vary. The sample illustration is original project artwork licensed with the code under MIT. It contains no participant data or real-world measurements.

`core.js` defines the review/report contract; `app.js` owns browser interactions; `style.css` owns presentation. Add model proposals behind a separate reviewed interface, not by relabeling sample observations as predictions. Insert user text through text nodes, never HTML. Keep the no-upload policy intact.

Canonical [project status](../docs/status/PROJECT_SCORECARD.md) and [machine status](../evaluation/status.json) describe the separate autonomy program. [Start Here](../docs/08-contributors/start-here.md) describes onboarding and [pilot tasks](../docs/08-contributors/pilot/backlog.md) describe the next contributions.
