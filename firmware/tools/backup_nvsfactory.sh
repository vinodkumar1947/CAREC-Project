#!/usr/bin/env bash
# backup_nvsfactory.sh — Back up the Watcher's factory partition before any flash
#
# The nvsfactory partition (0x9000, 200 KB) contains the device EUI and factory
# calibration data. It is NOT restored by a firmware update — if erased, the
# device may lose its SenseCraft cloud identity.
#
# Run this ONCE before any idf.py flash (not app-flash) operation.
# Output: firmware/backups/nvsfactory_<MAC>.bin
#
# Usage:
#   ./firmware/tools/backup_nvsfactory.sh [PORT]

set -euo pipefail

FIRMWARE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BACKUP_DIR="$FIRMWARE_DIR/backups"
mkdir -p "$BACKUP_DIR"

if [ $# -ge 1 ]; then
    PORT="$1"
else
    PORT=$(ls /dev/tty.wchusbserial* /dev/cu.usbmodem* /dev/ttyACM* 2>/dev/null | head -1 || true)
    if [ -z "$PORT" ]; then
        echo "ERROR: No serial port found. Connect Watcher via bottom/side USB-C."
        exit 1
    fi
fi

OUTFILE="$BACKUP_DIR/nvsfactory_$(date +%Y%m%d_%H%M%S).bin"

echo "=== CAREC nvsfactory Backup ==="
echo "Port   : $PORT"
echo "Output : $OUTFILE"
echo ""
echo "Reading 200 KB from 0x9000..."

esptool.py \
    --port "$PORT" \
    --baud 2000000 \
    --chip esp32s3 \
    --before default_reset \
    --after hard_reset \
    --no-stub \
    read_flash 0x9000 204800 "$OUTFILE"

echo ""
echo "Backup saved: $OUTFILE"
echo "Keep this file safe — it cannot be regenerated."
