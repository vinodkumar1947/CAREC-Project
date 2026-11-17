# Getting Started

Start with [the contributor entry page](start-here.md) to try CAREC and choose a contribution track. Hardware is not required.

## Browser pilot

Open `pilot/index.html` in a desktop browser, or serve the pilot from the repository root:

```bash
python3 scripts/contributor_check.py --track pilot
python3 -m http.server 8000 --bind 127.0.0.1 --directory pilot
```

Open `http://127.0.0.1:8000`. This needs no Python packages, ROS, Docker, account, or API key. On Windows use `py -3` in place of `python3`.

## Portable Python reference tests

Requires Git and Python 3.10+ with virtual-environment support. From the checkout root on macOS/Linux:

```bash
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements-dev.txt
python scripts/contributor_check.py --track python
python -m pytest tests/unit -v
```

Windows PowerShell:

```powershell
py -3 -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install -r requirements-dev.txt
python scripts/contributor_check.py --track python
python -m pytest tests/unit -v
```

If activation is restricted, use `.venv\Scripts\python.exe` directly for the last three commands. The reference tests do not launch Gazebo or establish physical-wheelchair safety.

## ROS simulation engineering

Follow [Development Environment](development-environment.md). The ROS container is a separate track, not a prerequisite for the browser pilot. Existing platform validation issues remain open until evidence is provided.

## Before your pull request

Run the checks relevant to your issue. Documentation contributors run:

```bash
python3 scripts/check_markdown_links.py
```

Pilot JavaScript contributors also run:

```bash
node --test tests/pilot/review.test.cjs
```

Follow [Start Here](start-here.md) for task claiming, public questions, review routing, and troubleshooting. Never submit participant footage, private notes, or credentials as test evidence.
