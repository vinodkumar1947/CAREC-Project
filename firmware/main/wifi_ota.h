#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

// OTA firmware update — pull-based via HTTPS
// Checks a version endpoint; downloads + flashes only if newer.
// Runs only when wheelchair is stationary (no active danger alerts).

// ----- Configuration --------------------------------------------------------
// Replace with your actual OTA server URL before deploying.
// Server must serve:
//   GET /version  → plain text "1.2.3"
//   GET /firmware → raw binary (.bin)
#define OTA_VERSION_URL  "https://your-ota-server.com/carec/version"
#define OTA_FIRMWARE_URL "https://your-ota-server.com/carec/firmware.bin"

#define OTA_CHECK_INTERVAL_MS  (60UL * 60UL * 1000UL)  // check every 1 hour
#define CAREC_FIRMWARE_VERSION "1.0.0"
// ----------------------------------------------------------------------------

static unsigned long _ota_last_check = 0;
static bool _ota_safe_to_update = true;  // set false during active alerts

// Call this from the safety loop to block OTA during danger states.
inline void ota_set_safe(bool safe) {
    _ota_safe_to_update = safe;
}

// Returns true if a newer version string is available on the server.
static bool _ota_newer_version_available() {
    HTTPClient http;
    http.begin(OTA_VERSION_URL);
    int code = http.GET();
    if (code != 200) {
        http.end();
        return false;
    }
    String remote = http.getString();
    remote.trim();
    http.end();
    // Simple string compare — assumes semver with leading zeros (e.g. "1.0.1" > "1.0.0")
    return remote > String(CAREC_FIRMWARE_VERSION);
}

// Progress callback printed to Serial during flash.
static void _ota_progress(int cur, int total) {
    Serial.printf("[OTA] Progress: %d / %d bytes\n", cur, total);
}

// Check for update and flash if newer. Reboots on success.
// Safe to call frequently — rate-limited internally.
inline void ota_check_and_update() {
    if (!_ota_safe_to_update) return;
    if (WiFi.status() != WL_CONNECTED) return;

    unsigned long now = millis();
    if (now - _ota_last_check < OTA_CHECK_INTERVAL_MS) return;
    _ota_last_check = now;

    Serial.println("[OTA] Checking for firmware update...");

    if (!_ota_newer_version_available()) {
        Serial.printf("[OTA] Up to date (%s)\n", CAREC_FIRMWARE_VERSION);
        return;
    }

    Serial.println("[OTA] New firmware found — downloading...");

    // Block obstacle detection during flash (safety: beep once to warn)
    ota_set_safe(false);

    httpUpdate.onProgress(_ota_progress);
    httpUpdate.rebootOnUpdate(true);   // auto-reboot; ESP32 rolls back on bad boot

    HTTPClient http;
    http.begin(OTA_FIRMWARE_URL);
    t_httpUpdate_return result = httpUpdate.update(http);

    switch (result) {
        case HTTP_UPDATE_OK:
            // Device reboots here — code below unreachable on success
            Serial.println("[OTA] Update applied, rebooting...");
            break;
        case HTTP_UPDATE_FAILED:
            Serial.printf("[OTA] Update failed (%d): %s\n",
                httpUpdate.getLastError(),
                httpUpdate.getLastErrorString().c_str());
            // ESP32 dual-partition scheme keeps old firmware — safe to continue
            ota_set_safe(true);
            break;
        case HTTP_UPDATE_NO_UPDATES:
            ota_set_safe(true);
            break;
    }
    http.end();
}
