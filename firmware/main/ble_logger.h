#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "motion_detector.h"

// BLE GATT event logger — SenseCAP Watcher W1-A (ESP32-S3)
//
// Implements a Nordic UART Service (NUS) GATT server. The caregiver's phone
// subscribes to the TX characteristic and receives real-time JSON obstacle
// alerts. Any BLE scanner app (nRF Toolbox, LightBlue, Serial Bluetooth
// Terminal) can receive events without a custom app.
//
// Event format (JSON, ≤ 512 bytes):
//   {"zone":"RED","dist_cm":42.3,"motion":"FORWARD","ts_ms":12345678}
//
// UUIDs — Nordic UART Service (NUS):
//   Service:    6E400001-B5A3-F393-E0A9-E50E24DCCA9E
//   TX (notify): 6E400003-B5A3-F393-E0A9-E50E24DCCA9E  ← phone reads events here
//   RX commands are intentionally not exposed. Safety-relevant configuration
//   requires an authenticated, authorized protocol that is not implemented.
//
// Public API:
//   ble_logger_init()                    — call once in setup()
//   ble_log_event(dist_cm, zone, motion) — call each detection loop when moving
//   ble_log_raw(msg)                     — send arbitrary string (debug/custom)
//
// When no client is connected, events are mirrored to Serial so bench testing
// works without a phone present.

// ── UUIDs ────────────────────────────────────────────────────────────────────

#define BLE_NUS_SERVICE_UUID  "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLE_NUS_TX_CHAR_UUID  "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLE_DEVICE_NAME       "CAREC-Watcher"

// ── State ────────────────────────────────────────────────────────────────────

static BLEServer*         _ble_server    = nullptr;
static BLECharacteristic* _ble_tx_char  = nullptr;
static bool               _ble_client   = false;   // true when caregiver phone connected

// ── Server callbacks — handle connect / disconnect ───────────────────────────

class _CARECBLECallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer*) override {
        _ble_client = true;
        Serial.println("[BLE] Caregiver app connected");
    }
    void onDisconnect(BLEServer*) override {
        _ble_client = false;
        Serial.println("[BLE] Caregiver app disconnected — restarting advertising");
        BLEDevice::startAdvertising();
    }
};

// ── Helpers ──────────────────────────────────────────────────────────────────

static const char* _zone_str(int zone) {
    if (zone == 2) return "RED";
    if (zone == 1) return "YELLOW";
    return "GREEN";
}

static const char* _motion_str(MotionState m) {
    if (m == MOTION_FORWARD)  return "FORWARD";
    if (m == MOTION_BACKWARD) return "BACKWARD";
    return "STATIONARY";
}

// Send JSON string over BLE notify. Falls back to Serial when no client.
static void _ble_notify(const char* json) {
    Serial.printf("[BLE] %s\n", json);  // always mirror to Serial for bench use
    if (!_ble_client || !_ble_tx_char) return;
    _ble_tx_char->setValue((uint8_t*)json, strlen(json));
    _ble_tx_char->notify();
}

// ── Public API ────────────────────────────────────────────────────────────────

inline void ble_logger_init() {
    BLEDevice::init(BLE_DEVICE_NAME);

    _ble_server = BLEDevice::createServer();
    _ble_server->setCallbacks(new _CARECBLECallbacks());

    BLEService* svc = _ble_server->createService(BLE_NUS_SERVICE_UUID);

    // TX: device → phone (notify on each obstacle event)
    _ble_tx_char = svc->createCharacteristic(
        BLE_NUS_TX_CHAR_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    _ble_tx_char->addDescriptor(new BLE2902());

    svc->start();

    BLEAdvertising* adv = BLEDevice::getAdvertising();
    adv->addServiceUUID(BLE_NUS_SERVICE_UUID);
    adv->setScanResponse(true);
    adv->setMinPreferred(0x06);   // helps iOS connect reliably
    BLEDevice::startAdvertising();

    Serial.printf("[BLE] Advertising as \"%s\" (Nordic UART Service)\n", BLE_DEVICE_NAME);
}

// Send a structured obstacle event.
inline void ble_log_event(float dist_cm, int zone, MotionState motion) {
    char buf[128];
    snprintf(buf, sizeof(buf),
             "{\"zone\":\"%s\",\"dist_cm\":%.1f,\"motion\":\"%s\",\"ts_ms\":%lu}",
             _zone_str(zone), dist_cm, _motion_str(motion), millis());
    _ble_notify(buf);
}

// Send a raw string (debug / future custom messages).
inline void ble_log_raw(const char* msg) {
    _ble_notify(msg);
}
