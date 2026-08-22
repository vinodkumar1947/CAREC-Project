# CAREC Mobile Integration

**App:** SenseCraft Mate (iOS / Android — free)  
**Protocol:** Bluetooth Low Energy (BLE 5) GATT

---

## Overview

CAREC sends real-time obstacle events to the **SenseCraft Mate** caregiver app over BLE. The caregiver's phone receives push notifications for each zone transition (GREEN → YELLOW, GREEN → RED, etc.).

---

## App Download

| Platform | Link |
|----------|------|
| iOS | https://apps.apple.com/us/app/sensecraft/id1619944834 |
| Android | https://play.google.com/store/apps/details?id=cc.seeed.sensecapmate |

---

## Pairing Steps

1. Power on the SenseCAP Watcher (hold button 3 seconds)
2. Open SenseCraft Mate → tap **"Add Device"**
3. Select **"SenseCAP Watcher W1-A"** from the scan list
4. Follow the in-app pairing wizard
5. Configure 2.4 GHz WiFi in the app (required for OTA)
6. Verify "CAREC" appears in the device list with a green status dot

---

## BLE Event Format

Each obstacle event is a JSON string sent via BLE GATT notify:

```json
{"zone":"RED","dist_cm":42.3,"motion":"FORWARD","ts_ms":12345678}
```

| Field | Type | Values |
|-------|------|--------|
| `zone` | string | `"GREEN"`, `"YELLOW"`, `"RED"` |
| `dist_cm` | float | distance to nearest obstacle (cm) |
| `motion` | string | `"STATIONARY"`, `"FORWARD"`, `"BACKWARD"` |
| `ts_ms` | uint32 | `millis()` timestamp since boot (ms) |

---

## BLE GATT Service (Planned)

> **Current status:** Stub — events are mirrored to Serial until UUIDs are confirmed with Seeed.

| Role | UUID |
|------|------|
| Service (Nordic UART) | `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` |
| TX Characteristic (notify) | `6E400003-B5A3-F393-E0A9-E50E24DCCA9E` |

**Action required:** Contact Seeed Studio to confirm whether SenseCraft Mate uses the Nordic UART UUIDs above or a custom CAREC-specific characteristic. See `firmware/main/ble_logger.h` for the stub implementation and `TODO` comments.

---

## Implementing Real BLE GATT

When UUIDs are confirmed, replace the stub in `firmware/main/ble_logger.h`:

```cpp
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic* pTxChar;

inline void ble_logger_init() {
    BLEDevice::init("CAREC-Watcher");
    BLEServer* pServer = BLEDevice::createServer();
    BLEService* pService = pServer->createService(SERVICE_UUID);
    pTxChar = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pTxChar->addDescriptor(new BLE2902());
    pService->start();
    BLEDevice::startAdvertising();
    _ble_ready = true;
}

static void _ble_notify(const char* json) {
    if (!_ble_ready) return;
    pTxChar->setValue((uint8_t*)json, strlen(json));
    pTxChar->notify();
}
```

---

## Home Assistant Integration (Phase 2 — Week 7-8)

CAREC will forward BLE events to Home Assistant via Node-RED:

```
Watcher BLE → Node-RED MQTT bridge → Home Assistant → Dashboard alert
```

See `docs/specifications/system_spec.md` for full architecture.
