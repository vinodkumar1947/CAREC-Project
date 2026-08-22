# CAREC Setup Guide

**Time to first test:** ~30 minutes  
**Audience:** New contributors, hardware evaluators, makers building their own CAREC unit  
**Last updated:** May 19, 2026

This guide gets you from zero to a running CAREC firmware build and a working obstacle detection test. For full developer documentation see [`docs/guides/programming_guide.md`](programming_guide.md).

---

## What You Need

### Software
- macOS or Linux host (Windows via WSL2 is untested)
- Python 3.11 (3.9 is not supported — symlinks break on some Mac setups)
- Git
- ESP-IDF v5.3.5 — see Step 1 below

### Hardware
- SenseCAP Watcher W1-A (Clear Enclosure edition, SKU 113991315)
- USB-C cable (data-capable, not charge-only)
- A Mac or Linux machine to flash from

---

## Step 1 — Install ESP-IDF v5.3.5

If you already have ESP-IDF v5.3.5, skip to Step 2.

```bash
git clone -b v5.3.5 --recursive https://github.com/espressif/esp-idf.git ~/esp/esp-idf
cd ~/esp/esp-idf && ./install.sh esp32s3
```

Verify:

```bash
. ~/esp/esp-idf/export.sh
idf.py --version   # should print 5.3.5
```

---

## Step 2 — Set Up Python 3.11 Virtual Environment

CAREC uses a separate Python venv for host-side tools and tests. Do this once.

```bash
python3.11 -m venv ~/idf5.3_py3.11_env
source ~/idf5.3_py3.11_env/bin/activate
pip install pytest pyserial esptool
```

> **Note:** If you previously had a Python 3.9 venv (`idf5.3_py3.9_env`), ignore it — it is broken if Anaconda was removed from your system.

---

## Step 3 — Clone the Repository

```bash
git clone https://github.com/vinodkumar1947/CAREC.git
cd CAREC
```

---

## Step 4 — Build the Firmware

```bash
cd firmware
. ~/esp/esp-idf/export.sh          # activate ESP-IDF
source ~/idf5.3_py3.11_env/bin/activate  # activate Python env

idf.py set-target esp32s3          # one-time per checkout
idf.py build
```

A successful build ends with:

```
CAREC_firmware.bin  1.64 MB  (46% of 3 MB partition free)
Project build complete.
```

If you see errors, check:
- ESP-IDF version is exactly 5.3.5 (`idf.py --version`)
- Python env is activated (`which python` should point to the venv)
- Target was set to `esp32s3` (`idf.py set-target esp32s3`)

---

## Step 5 — Find the Device Port

Connect the SenseCAP Watcher via USB-C and find its port:

```bash
# macOS
ls /dev/cu.usbmodem*    # look for a new entry after plugging in

# Linux
ls /dev/ttyUSB* /dev/ttyACM*
```

The port is usually `/dev/cu.usbmodem<number>` on macOS.

---

## Step 6 — Flash the Firmware

```bash
idf.py -p /dev/cu.usbmodem<YOUR_PORT> flash monitor
```

On successful flash you will see:

```
I (xxx) CAREC: carec_setup() complete
I (xxx) CAREC: CAREC ready.
```

Press `Ctrl+]` to exit the monitor.

---

## Step 7 — Verify the System

With the firmware running:

1. **Green display / silence:** Power-on idle state. Camera is watching, wheelchair treated as stationary.

2. **Trigger a WARNING zone:** Hold your hand 80 cm from the camera.
   - Expected: Yellow display + one slow repeating beep within 200ms.

3. **Trigger a CRITICAL zone:** Hold your hand 30 cm from the camera.
   - Expected: Red display + 3× fast repeating beeps within 200ms.

4. **Clear zone:** Remove your hand entirely.
   - Expected: Green display + silence within 200ms.

If alerts don't fire, check the serial monitor for detection output:

```
I (xxx) DETECT: person conf=0.82 bbox_w=0.41 → CRITICAL
I (xxx) BEEP: CRITICAL pattern triggered
```

---

## Step 8 — Connect the Mobile App (optional)

1. Download **SenseCraft Mate** (free):
   - iOS: App Store → search "SenseCraft"
   - Android: Play Store → search "SenseCraft Mate"

2. Open the app → Bluetooth scan → select your CAREC device

3. You should see zone events appearing in the device log within seconds of detection

---

## Step 9 — Connect to WiFi for OTA (optional)

In the SenseCraft Mate app:

1. Open device settings → WiFi configuration
2. Enter your 2.4 GHz network SSID and password
3. Device reboots and connects — green LED confirms WiFi connected
4. OTA update check runs automatically on next boot

---

## Troubleshooting

| Symptom | Likely cause | Fix |
|---------|-------------|-----|
| Build fails: `python: command not found` | Wrong Python env active | `source ~/idf5.3_py3.11_env/bin/activate` |
| Flash fails: `No such file or directory` | Wrong port | Re-check `ls /dev/cu.usbmodem*` with device plugged in |
| No beep on obstacle | Speaker not initialised | Check serial: look for `ES8311 init OK` — if missing, check USB-C cable |
| Display stays dark | SPD2010 LCD hardware issue (known) | Not a firmware bug — RGB LED is the current primary visual indicator |
| BLE not visible in app | Bluetooth not initialised | Check serial for `BLE init OK`; power-cycle device |
| `containsKey()` warnings at build | Vendored ArduinoJson API (upstream) | Safe to ignore — not CAREC code |

---

## Run the Host-Side Tests

```bash
cd tests/
source ~/idf5.3_py3.11_env/bin/activate
python -m pytest -v
```

All tests should pass before submitting a pull request.

---

## Next Steps

- **Developer reference:** [`docs/guides/programming_guide.md`](programming_guide.md) — deep-dive into the firmware architecture and development workflow
- **API reference:** [`docs/api/firmware_api.md`](../api/firmware_api.md) — all module public functions and types
- **Architecture:** [`docs/ARCHITECTURE.md`](../ARCHITECTURE.md) — system design and data flow
- **Contributing:** [`CONTRIBUTING.md`](../../CONTRIBUTING.md) — how to submit improvements
