#!/usr/bin/env python3
"""Check the selected contributor track without claiming the whole stack is ready."""
import argparse
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--track', choices=['pilot', 'python', 'ros'], default='pilot')
    args = parser.parse_args()
    missing = []
    if sys.version_info < (3, 10):
        missing.append('Python 3.10+ (run this command with that interpreter)')
    if not shutil.which('git'):
        missing.append('Git')
    if args.track == 'pilot':
        for name in ['index.html', 'style.css', 'core.js', 'app.js']:
            if not (ROOT / 'pilot' / name).is_file():
                missing.append(f'pilot/{name}; run from a complete checkout')
    elif args.track == 'python':
        try:
            import pytest  # noqa: F401
        except ImportError:
            missing.append('pytest; install requirements-dev.txt in your virtual environment')
    else:
        for name in ['ros2', 'colcon']:
            if not shutil.which(name):
                missing.append(f'{name}; use the documented ROS Dev Container')
    if missing:
        print('Not ready for ' + args.track + ':\n- ' + '\n- '.join(missing))
        return 1
    print(f'{args.track} prerequisites found. This does not validate ROS simulation or browser compatibility.')
    if args.track == 'pilot':
        print('Open pilot/index.html, or run: python3 -m http.server 8000 --bind 127.0.0.1 --directory pilot')
        if shutil.which('node'):
            return subprocess.call(['node', '--test', 'tests/pilot/review.test.cjs'], cwd=ROOT)
        print('Node.js is optional for trying the demo; install Node.js 22+ to run JavaScript contract tests.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
