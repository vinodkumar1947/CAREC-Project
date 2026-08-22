#!/usr/bin/env bash
# flash.sh — Build + flash CAREC firmware to SenseCAP Watcher W1-A
#
# Usage:
#   ./firmware/tools/flash.sh [PORT]
#   PORT defaults to auto-detect (/dev/tty.wchusbserial* or /dev/cu.usbmodem*)
#
# Prerequisites:
#   - ESP-IDF v5.2.1 installed and sourced: get_idf
#   - SenseCAP Watcher connected via bottom/side USB-C (data port)
#   - wifi_config.h created from template (see firmware/config/)
#
# SAFE FLASH: uses  idf.py app-flash  — only flashes the app binary.
# This preserves nvsfactory (device EUI) at 0x9000 on the Watcher.
# Never run  idf.py flash  (full flash) on this device.

set -euo pipefail

FIRMWARE_DIR="$(cd "$(dirname "$0")/.." && pwd)"

# ── Auto-detect serial port ───────────────────────────────────────────────────
if [ $# -ge 1 ]; then
    PORT="$1"
else
    PORT=$(ls /dev/tty.wchusbserial* /dev/cu.usbmodem* /dev/ttyACM* 2>/dev/null | head -1 || true)
    if [ -z "$PORT" ]; then
        echo "ERROR: No serial port found."
        echo "  • Connect Watcher via the bottom/side USB-C port (not the back port)"
        echo "  • Or run: ./firmware/tools/flash.sh /dev/tty.wchusbserialXXXX"
        exit 1
    fi
fi

echo "=== CAREC Flash Tool (ESP-IDF) ==="
echo "Firmware: $FIRMWARE_DIR"
echo "Port    : $PORT"
echo ""

# ── Verify IDF environment ────────────────────────────────────────────────────
if [ -z "${IDF_PATH:-}" ]; then
    echo "ERROR: ESP-IDF environment not active."
    echo "  Run:  get_idf   (or source ~/esp/esp-idf/export.sh)"
    exit 1
fi

# ── Ensure wifi_config.h exists ───────────────────────────────────────────────
CONFIG="$FIRMWARE_DIR/config/wifi_config.h"
if [ ! -f "$CONFIG" ]; then
    echo "WARNING: wifi_config.h not found — copying from template."
    echo "  Edit $CONFIG with your 2.4 GHz credentials."
    cp "$FIRMWARE_DIR/config/wifi_config.h.template" "$CONFIG"
fi

# ── Build ─────────────────────────────────────────────────────────────────────
echo "[1/2] Building..."
cd "$FIRMWARE_DIR"
idf.py build

# ── Flash app only (safe — preserves nvsfactory) ──────────────────────────────
echo "[2/2] Flashing app to $PORT..."
idf.py --port "$PORT" -b 2000000 app-flash

echo ""
echo "Done! Run monitor to verify:"
echo "  ./firmware/tools/monitor.sh $PORT"
