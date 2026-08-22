# CAREC — Bill of Materials

**Hardware Decision:** SenseCAP Watcher W1-A Clear Enclosure (April 28, 2026)  
**Total Budget:** ~$103–127

---

## What Was Ordered

| Item | Model | Price | Purpose |
|------|-------|-------|---------|
| AI Camera + Speaker | SenseCAP Watcher W1-A Clear Enclosure (SKU 113991315) | $59.99 | Main device — camera, 1.45" display, speaker, mic, ESP32-S3, WiFi/BT |
| Battery | LiPo 10000mAh 3.7V | $15–25 | 50+ hours runtime |
| Angle mount | Ball Joint 1/4" thread | $10–15 | 360° + ±80° tilt adjustment |
| Armrest clamp | Tube Clamp 1–1.5" adj. | $8–12 | Non-invasive mount to wheelchair armrest |
| Storage (optional) | Micro-SD Card 32GB | $8–15 | Local data logging (FAT32, up to 32 GB) |

**Total (excl. optional SD): ~$93–112**  
**Total (incl. optional SD): ~$103–127**

---

## SenseCAP Watcher W1-A Clear Enclosure — What's Built In

All of these come with the device — no separate components needed:

| Feature | Spec |
|---------|------|
| Processor | ESP32-S3, dual-core 240 MHz, 8 MB PSRAM, 32 MB Flash |
| AI Accelerator | Himax WiseEye2 HX6538 (Cortex-M55 + Ethos-U55 NPU), 16 MB Flash |
| Camera | 5MP OV5647, 120° FOV, fixed focus 3m |
| Display | 1.45" touch screen, 412×412 px |
| Microphone | Built-in, 3m pickup radius |
| Speaker | Built-in, 1W output |
| Connectivity | WiFi 2.4GHz IEEE 802.11 b/g/n (100m) + Bluetooth 5 |
| Onboard Battery | Li-ion 3.7V, 400 mAh |
| Enclosure | Transparent (Clear), 69×65×20mm |
| USB | USB-C (power + programming) |
| Expansion | 1× Grove IIC + 2×4 GPIO headers |
| Storage | Micro-SD slot (up to 32 GB FAT32) |
| Integrations | SenseCraft, Home Assistant, Node-RED, XiaoZhi |

---

## Mounting Assembly

```
SenseCAP Watcher W1-A
    ↓ 1/4" thread
Ball Joint (360° rotation + ±80° tilt)
    ↓
Tube Clamp (1–1.5" diameter, stainless steel)
    ↓
Wheelchair Armrest
```

- Install time: 5 minutes
- Remove time: 2 minutes
- Damage to wheelchair: Zero

---

## Notes

- No breadboard, no wiring, no soldering — all-in-one device
- Firmware OTA via WiFi (no USB cable needed for updates)
- External battery pack extends runtime from ~6 hrs to 50+ hrs
- See [hardware/README.md](README.md) for mounting instructions
