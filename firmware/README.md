# CAREC Firmware Prototype

> **Owner-controlled legacy prototype.** This firmware provides experimental obstacle alerts only. It does not control wheelchair motion, is not a medical device, and is not part of the normal simulation contributor path. Only the project owner may flash or physically validate it.

## Purpose

The SenseCAP Watcher W1-A prototype explores on-device object detection, approximate monocular distance estimation, and local audio, LED, display, BLE, and OTA behavior. It is retained as research input for future perception and hardware-adapter decisions.

## Hardware and toolchain

- SenseCAP Watcher W1-A
- ESP32-S3 with Himax HX6538 inference coprocessor
- ESP-IDF v5.3.5
- Arduino ESP32 as a managed ESP-IDF component
- C++17
- Seeed SSCMA driver pinned as a Git submodule

## Source map

```text
firmware/
├── CMakeLists.txt
├── partitions.csv
├── sdkconfig.defaults
├── config/
│   ├── build_flags.h
│   └── zone_config.h
├── main/
│   ├── CAREC_main.cpp
│   ├── safety_decision.h
│   ├── sensecraft_detection.h
│   ├── distance_estimator.h
│   ├── directional_beep_patterns.h
│   ├── display_alert.h
│   ├── led_status.h
│   ├── motion_detector.h
│   ├── ble_logger.h
│   └── wifi_ota.h
├── components/Seeed_Arduino_SSCMA/
├── tools/
└── releases/
```

## Actual decision path

```text
SSCMA invocation
    ├── unavailable or inference failed → RED fail-safe, alert, OTA blocked
    └── healthy
          ├── detections → nearest approximate distance → zone
          └── no detections → GREEN

zone → speaker + RGB LED + display + BLE event + OTA gate
```

The production safety boundary is the portable function in `main/safety_decision.h`. A healthy empty frame and a failed detector are deliberately different states.

## Known limitations

- Distance is inferred from object bounding-box width and assumed class width; it is not measured depth.
- The ESP32 does not receive raw camera frames from the Himax path, so `motion_detector.h` is currently an always-forward stub. No optical-flow gate is active.
- Detection coverage is limited to classes supported by the loaded model.
- Thin, transparent, reflective, low, overhanging, and unusual objects are not proven detectable.
- Display operation remains unresolved on the prototype.
- BLE and OTA behavior is experimental and not a safety channel.
- No completed obstacle matrix, battery-runtime validation, or occupied-wheelchair deployment approval exists.

## Safety behavior

| Detector state | Distance input | Result |
|---|---:|---|
| Healthy | Below 60 cm | RED |
| Healthy | 60–100 cm | YELLOW |
| Healthy | 100 cm or farther | GREEN |
| Healthy, no supported detection | 200 cm sentinel | GREEN, known perception limitation |
| Not ready or inference failure | Ignored | RED fail-safe |

These thresholds are prototype configuration values, not validated braking distances.

## Build and tests

Initialize the dependency and follow [SETUP.md](SETUP.md):

```bash
git submodule update --init --recursive
cd firmware
idf.py build
```

The host-side portable safety test is compiled in `.github/workflows/test.yml`. Python tests cover legacy distance, zone, alert, and event behavior. See [tests/README.md](../tests/README.md).

## Test modes

`CAREC_main.cpp` contains mutually exclusive display, LED, and speaker bring-up modes. They are for the project owner and must not be presented as completed system validation.

## Current relationship to CAREC Sim

The simulation program does not depend on this device. Future physical integration must use an owner-controlled adapter behind the independent safety supervisor; community autonomy code may never call this firmware as a motor interface.
