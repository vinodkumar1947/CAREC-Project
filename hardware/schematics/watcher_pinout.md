# SenseCAP Watcher W1-A — GPIO & Connector Reference

> Hardware: SenseCAP Watcher W1-A Clear Enclosure (SKU 113991315)  
> Processor: ESP32-S3 (dual-core 240 MHz, 8 MB PSRAM, 32 MB Flash)

---

## External Connectors

| Connector | Type | Notes |
|-----------|------|-------|
| USB-C | Power + data | 5V input, USB CDC serial (115200 baud) |
| Grove IIC | 4-pin JST (3.3V) | SDA/SCL + 3.3V + GND |
| GPIO Header | 2×4 pin 2.54mm | See GPIO map below |
| Micro-SD | Full-size slot | FAT32, up to 32 GB |

---

## Built-in Hardware Map

| Peripheral | GPIO / Bus | Notes |
|------------|------------|-------|
| OV5647 Camera | CSI lane (to HX6538) | 5MP, 120° FOV, fixed focus 3m |
| Himax HX6538 NPU | I2C + SPI (internal) | Connected to ESP32-S3 internally |
| 1.45" Display | SPI (internal) | 412×412 px, touch controller |
| Speaker amplifier | GPIO 4 (confirmed TBD) | 1W, PWM-driven — **verify when hardware arrives** |
| Microphone | I2S (internal) | 3m pickup radius |
| RGB LED | GPIO (internal) | Status indicator |
| Button | GPIO 0 (boot) | Hold 3s to enter boot mode |

> **TODO:** Confirm `SPEAKER_PIN` GPIO number from Seeed hardware docs or oscilloscope trace when device arrives.

---

## Grove IIC Port Pinout

```
Pin 1 — SDA   (GPIO 5 on ESP32-S3)
Pin 2 — SCL   (GPIO 6 on ESP32-S3)
Pin 3 — VCC   (3.3V, max 300mA)
Pin 4 — GND
```

Compatible Phase 2 sensors:
- Grove ADXL345 Accelerometer (motion gate)
- Grove HC-SR04 Ultrasonic (rear detection)
- Grove IR Cliff Sensor (drop-off detection)

---

## GPIO Expansion Header (2×4)

| Pin | Signal | Direction | Notes |
|-----|--------|-----------|-------|
| 1 | 3.3V | PWR | 300mA max |
| 2 | GND | GND | |
| 3 | GPIO 42 | I/O | General purpose |
| 4 | GPIO 41 | I/O | General purpose |
| 5 | GPIO 40 | I/O | General purpose |
| 6 | GPIO 39 | I/O | General purpose |
| 7 | GPIO 38 | I/O | General purpose |
| 8 | GPIO 37 | I/O | General purpose |

> Pinout to be verified against Seeed OSHW repo schematic: https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher

---

## Power Budget

| Mode | Current Draw | Notes |
|------|-------------|-------|
| Full operation (WiFi + camera + display) | 200–280 mA | Typical |
| Camera only (no WiFi) | 150–180 mA | After `WiFi.disconnect()` |
| Deep sleep | ~2 mA | Future feature |
| Peak (WiFi TX burst) | up to 500 mA | Brief spikes |

**Battery life with 10,000 mAh LiPo:** 50+ hours at typical draw.
