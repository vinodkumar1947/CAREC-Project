Open the IDF serial monitor for the CAREC firmware (ESP32-S3 console).

**Port:** `/dev/cu.usbmodem5B142665543` (higher-numbered — ESP32-S3)
Note: the lower-numbered port is the Himax WE2 console — do not use it for this.

```bash
export PATH="/Users/vinod/.espressif/python_env/idf5.3_py3.11_env/bin:$PATH"
source ~/esp/esp-idf/export.sh 2>/dev/null
idf.py -C /Users/vinod/Documents/GitHub/CAREC-Project/firmware \
        -B /Users/vinod/Documents/GitHub/CAREC-Project/firmware/build \
        --port /dev/cu.usbmodem5B142665543 monitor
```

Exit with `Ctrl+]`. Key log prefixes to watch:
- `[Safety]` — zone classification and distance
- `[Detection]` — SSCMA bounding-box results
- `[SSCMA Raw]` — raw invoke diagnostics
- `[Audio]` — ES8311 codec status
- `[BLE]` — caregiver alert events
- `[OTA]` — firmware update checks
