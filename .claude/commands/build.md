Build the CAREC firmware with ESP-IDF v5.3.5.

```bash
export PATH="/Users/vinod/.espressif/python_env/idf5.3_py3.11_env/bin:$PATH"
source ~/esp/esp-idf/export.sh 2>/dev/null
idf.py -C /Users/vinod/Documents/GitHub/CAREC-Project/firmware \
        -B /Users/vinod/Documents/GitHub/CAREC-Project/firmware/build \
        build 2>&1 | grep -E "(error:|warning:|Built target|binary size|free|Project build)"
```

Report: zero-error confirmation, binary size, and percentage of partition used.
If there are errors, show the full compiler error with file and line number.
