Flash CAREC_firmware.bin to the SenseCAP Watcher W1-A.

**Port:** `/dev/cu.usbmodem5B142665543` (the higher-numbered USB port — ESP32-S3 flash)
**Baud:** 2 Mbaud

```bash
export PATH="/Users/vinod/.espressif/python_env/idf5.3_py3.11_env/bin:$PATH"
source ~/esp/esp-idf/export.sh 2>/dev/null
idf.py -C /Users/vinod/Documents/GitHub/CAREC-Project/firmware \
        -B /Users/vinod/Documents/GitHub/CAREC-Project/firmware/build \
        --port /dev/cu.usbmodem5B142665543 -b 2000000 flash
```

If the port is not found, list available ports with:
```bash
ls /dev/cu.usbmodem* 2>/dev/null
```
The W1-A enumerates two ports — use the higher-numbered one for flashing.
