# Start Here — CAREC Contributor Pilot

Status: Proposed in [pilot implementation #53](https://github.com/vinodkumar1947/CAREC-Project/issues/53). Broad recruitment awaits independent review, resolution of the pre-existing [traceability blocker #59](https://github.com/vinodkumar1947/CAREC-Project/issues/59), and two newcomer walkthroughs.

CAREC develops open software for wheelchair accessibility and mobility research. You can contribute using your computer. No custom hardware, paid API key, private introduction, or device account is needed for this pilot.

## Try something first

The accessibility review pilot lets you review authored sample observations, annotate a locally selected video, confirm or dismiss findings, export JSON, and print a report. **There is no automatic AI detection yet.** The sample is an illustration, not measured footage. The app neither determines safe passage nor controls a wheelchair.

1. Open the [repository](https://github.com/vinodkumar1947/CAREC-Project). While #53 is unmerged, select branch `feature/CARE-053`.
2. Use **Code → Download ZIP**, extract it, and open `pilot/index.html` in a desktop browser. No installation is needed for this path.
3. Confirm the first finding and dismiss the second. The counter should show **2 / 3 reviewed**.
4. Export JSON. It should contain `automatic_inference: false` and your review choices.
5. Optionally select a video stored on your computer. The sample findings must disappear. Pause the video, add a timestamped observation, and export again.

Local video bytes are not uploaded or persisted by CAREC. Notes and filenames appear in exported reports. Refreshing or closing the tab clears the session. Browser codecs and local-file restrictions vary; use the localhost method below if necessary.

## Choose your contribution path

| Your skills or interests | Start with | Tools needed |
|---|---|---|
| HTML, CSS, JavaScript | Review interface, exports, keyboard access | Browser; Node.js 22+ for tests |
| Python/testing | Reference tests and contributor checks | Python 3.10+, Git |
| Accessibility or lived experience | Review usability and report confusing behavior | Browser; no coding required |
| Technical writing | Follow the guide and improve reproducibility | Browser/GitHub account |
| Robotics, C++, ROS | Existing simulation issues | [ROS development environment](development-environment.md) |
| AI/vision research | Proposed model adapters and benchmark design | Follow the specific issue; no model is required for the demo |

[Browse pilot tasks](pilot/backlog.md). Choose an unassigned issue whose dependencies are complete. Tasks dependent on #53 stay blocked until its PR is merged. Existing ROS work continues independently; choose by prerequisites rather than issue number.

## Run from a Git checkout

While #53 is unmerged:

```bash
git clone --branch feature/CARE-053 https://github.com/vinodkumar1947/CAREC-Project.git
cd CAREC-Project
python3 scripts/contributor_check.py --track pilot
python3 -m http.server 8000 --bind 127.0.0.1 --directory pilot
```

Open `http://127.0.0.1:8000`. Stop the server with Ctrl+C. After merge, clone normally without `--branch feature/CARE-053`.

On Windows PowerShell, use `py -3` instead of `python3`. The browser pilot does not require WSL, Docker, ROS, or a GPU. For JavaScript development, run:

```bash
node --test tests/pilot/review.test.cjs
```

For the portable Python reference tests, use the [Getting Started guide](getting-started.md). For ROS work, use the [separate environment guide](development-environment.md).

## Take a task and submit your change

1. Read the issue's outcome, starting files, acceptance criteria, and dependencies.
2. Comment “I am taking this task” on a ready, unassigned S0/S1 pilot issue. Under the proposed pilot policy, start a small change immediately; a maintainer resolves collisions in favor of the first public claim. S2/S3 work retains existing review requirements.
3. Fork the repository and create `feature/CARE-<issue-number>` or `docs/CARE-<issue-number>`. Use the actual GitHub issue number.
4. Make one focused change. Run the issue's verification steps.
5. Open a pull request against `main`. Link the issue, attach results, and describe limitations. If a prerequisite PR is unmerged, do not mark the dependent task ready.
6. Request review through the issue's named route. Authors cannot supply their own sole approval.

Read [CONTRIBUTING](../../CONTRIBUTING.md) for licensing, conduct, and review classes. Until this policy is merged, the current `main` contribution rules apply.

## Where questions go

- **Task question:** comment on that issue with what you tried, expected output, actual output, and environment.
- **Setup failure:** use the [bug form](https://github.com/vinodkumar1947/CAREC-Project/issues/new?template=bug.yml). Include the command and error; remove private filenames and personal information.
- **New idea:** use the feature form; new unscoped product work needs review before implementation.
- **Private security concern:** follow [SECURITY](../../SECURITY.md).

No personal email or direct message is required. The current review router is **@vinodkumar1947**. A separate technical maintainer has not yet been appointed; response times are not guaranteed. [Operating policy](pilot/operating-model.md) explains how this role can be delegated.

## Troubleshooting

| Symptom | Next action |
|---|---|
| Python command not found | Open `pilot/index.html` directly, or install Python 3.10+. On Windows try `py -3`. |
| Port 8000 already used | Choose 8001 in the server command and browser URL. |
| Blank or incomplete page from a local file | Serve the `pilot` directory with the localhost command; confirm all four files were extracted. |
| Video cannot play | Try a browser-supported MP4 or WebM. No automatic analysis runs. |
| Notes disappeared | Notes exist only in memory. Export before refreshing, closing, or changing sources. |
| `pytest` missing | Use a virtual environment and install `requirements-dev.txt`. |
| `ros2` missing | ROS is unnecessary for this pilot. Follow the ROS guide only for a ROS issue. |
| No ready tasks match your skills | Open a feature proposal describing a small outcome; do not start an unscoped rewrite. |

## Help us validate onboarding

Use the [newcomer evidence template](pilot/newcomer-validation.md). We need two people unfamiliar with the project to complete the demo and first-PR workflow without a private walkthrough. This is a release gate, not a result already achieved.

Canonical project status remains [the scorecard](../status/PROJECT_SCORECARD.md). Pilot progress does not increase the autonomy program's completion percentage.

See the [verification record](pilot/verification.md) for checks actually run and remaining limitations.
