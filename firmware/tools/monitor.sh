#!/usr/bin/env bash
# monitor.sh — Serial monitor for CAREC firmware (115200 baud)
#
# Usage:
#   ./firmware/tools/monitor.sh [PORT]
#   PORT defaults to auto-detect
#
# Press Ctrl+] to exit idf.py monitor.
# Output is also tee'd to logs/serial_TIMESTAMP.log

set -euo pipefail

FIRMWARE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
LOG_DIR="$FIRMWARE_DIR/../logs"
mkdir -p "$LOG_DIR"
LOGFILE="$LOG_DIR/serial_$(date +%Y%m%d_%H%M%S).log"

# ── Auto-detect port ─────────────────────────────────────────────────────────
if [ $# -ge 1 ]; then
    PORT="$1"
else
    PORT=$(ls /dev/tty.wchusbserial* /dev/cu.usbmodem* /dev/ttyACM* 2>/dev/null | head -1 || true)
    if [ -z "$PORT" ]; then
        echo "ERROR: No serial port found. Connect Watcher via bottom/side USB-C."
        exit 1
    fi
fi

echo "=== CAREC Serial Monitor ==="
echo "Port   : $PORT"
echo "Log    : $LOGFILE"
echo "(Ctrl+] to exit)"
echo ""

if [ -n "${IDF_PATH:-}" ]; then
    idf.py --port "$PORT" monitor 2>&1 | tee "$LOGFILE"
elif command -v screen &>/dev/null; then
    screen "$PORT" 115200 | tee "$LOGFILE"
else
    echo "ERROR: Run 'get_idf' first, or install screen."
    exit 1
fi
