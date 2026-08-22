# Firmware Setup — SenseCAP Watcher W1-A

**Device:** SenseCAP Watcher W1-A (ESP32-S3 + Himax HX6538)
**Toolchain:** ESP-IDF v5.3.5 with arduino-esp32 as a managed component

---

## Prerequisites

- macOS, Linux, or Windows
- Python 3.11 (the ESP-IDF venv on this machine uses 3.11; system `python3` may be newer)
- USB-C cable (data-capable)
- SenseCAP Watcher W1-A device
- SenseCraft Mate app (iOS/Android) — for WiFi setup and OTA

---

## 1. Install ESP-IDF v5.3.5

```bash
git clone -b v5.3.5 --recursive https://github.com/espressif/esp-idf.git ~/esp/esp-idf
cd ~/esp/esp-idf
./install.sh esp32s3
```

## 2. Source the IDF environment

Prepend the Python 3.11 venv to PATH before sourcing `export.sh` — system `python3` may be 3.14+, which is not supported:

```bash
export PATH="$HOME/.espressif/python_env/idf5.3_py3.11_env/bin:$PATH"
. ~/esp/esp-idf/export.sh
```

If the `idf5.3_py3.11_env` venv doesn't exist yet (fresh machine), create it once:

```bash
python3.11 ~/esp/esp-idf/tools/idf_tools.py install-python-env
```

Then re-run the two lines above.

## 3. Build and flash

```bash
cd firmware
idf.py set-target esp32s3      # one-time per checkout
idf.py build
idf.py -p /dev/cu.usbmodem5B142665543 flash monitor
```

The W1-A enumerates **two** `usbmodem` ports on macOS — the higher-numbered one (`...5543`) is the IDF flash interface. The lower-numbered one is the Himax WE2 console.

For app-only re-flash (faster, skips bootloader/partition table):

```bash
idf.py -p /dev/cu.usbmodem5B142665543 app-flash
```

---

## Helper scripts

| Script | Purpose |
|---|---|
| `firmware/tools/flash.sh` | Convenience wrapper around `idf.py flash` |
| `firmware/tools/monitor.sh` | Opens `idf.py monitor` |
| `firmware/tools/backup_nvsfactory.sh` | Dumps the factory NVS partition before any destructive flash |

---

## Standalone hardware test

To verify toolchain + USB + onboard RGB LED in isolation, see [`RGB_LED_BRINGUP.md`](RGB_LED_BRINGUP.md). It builds the stock IDF blink example with the W1-A's WS2813 LED on GPIO 40.

---

## SenseCraft Mate (companion app)

The W1-A also accepts OTA from Seeed's SenseCAP Mate app. Useful for shipping firmware to non-developer users.

- iOS: https://apps.apple.com/us/app/sensecraft/id1619944834
- Android: https://play.google.com/store/apps/details?id=cc.seeed.sensecapmate

Reference firmware (BSP, schematics, sample drivers): https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher

---

## OTA pipeline

- Custom OTA implemented in `firmware/main/wifi_ota.h` using ESP-IDF `esp_https_ota`
- Trigger: automatic on WiFi reconnect (only in `ZONE_GREEN`), or via BLE command from the caregiver app

---

## First boot

After flashing, attach `idf.py monitor` (115200 baud) and reset. You should see:

```
I (xxx) main_task: Calling app_main()
CAREC Obstacle Detection starting...
```

Point the camera at an obstacle and verify detection events are logged.

---

## Troubleshooting

| Issue | Fix |
|-------|-----|
| Port not found | Check USB-C cable is data-capable (not charge-only). Confirm `ls /dev/cu.usbmodem*` shows two devices. |
| `idf.py: command not found` after `export.sh` | Prepend the py3.11 venv to PATH first — see step 2. |
| `export.sh` says venv doesn't exist | Anaconda was removed; run `python3.11 ~/esp/esp-idf/tools/idf_tools.py install-python-env` to recreate `idf5.3_py3.11_env`. |
| Build says "project configured with py3.9" | Run `idf.py fullclean` first, then build. Stale sdkconfig tracks the old venv name. |
| Upload fails / chip not detected | Hold BOOT button while plugging USB; release after esptool detects ESP32-S3. |
| Serial garbage | Baud must be 115200. |
| WiFi won't connect | Re-pair via SenseCraft Mate app, then trigger reconnect. |
| OTA fails | Verify WiFi is stable; check OTA server URL in `firmware/config/wifi_config.h`. |
| Camera no output | Restart device; verify Himax model is loaded (`sensecraft_detection.h` logs init result). |
