// CAREC Obstacle Detection — SenseCAP Watcher W1-A
// ESP32-S3 + SSCMA (Himax HX6538) + ES8311 + SPD2010 QSPI LCD

// Build-time test modes — each takes over app_main entirely.
// Uncomment ONE; leave all commented for the full CAREC firmware.
// #define DISPLAY_TEST       // SPD2010 LCD RED → YELLOW → GREEN cycle (no SSCMA/BLE/WiFi)
// #define LED_TEST           // WS2813 RGB LED color cycle on GPIO 40 (no SSCMA/BLE/WiFi/LCD)
// #define SPEAKER_TEST       // ES8311 speaker bring-up: heartbeat beep once per minute

#if (defined(DISPLAY_TEST) + defined(LED_TEST) + defined(SPEAKER_TEST)) > 1
#error "Enable at most one of DISPLAY_TEST / LED_TEST / SPEAKER_TEST"
#endif

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../config/build_flags.h"
#include "../config/zone_config.h"

#if defined(SPEAKER_TEST)
#include <Wire.h>
#include "speaker_test.h"
#elif defined(LED_TEST)
#include "rgb_led_test.h"
#else
#include <Wire.h>
#include "display_alert.h"
#endif

#if !defined(DISPLAY_TEST) && !defined(LED_TEST) && !defined(SPEAKER_TEST)
#include <WiFi.h>
#include "sensecraft_detection.h"
#include "distance_estimator.h"
#include "safety_decision.h"
#include "directional_beep_patterns.h"
#include "motion_detector.h"
#include "ble_logger.h"
#include "wifi_ota.h"
#include "led_status.h"
#endif

#ifdef SPEAKER_TEST

extern "C" void app_main() {
    initArduino();
    Serial.begin(115200);
    delay(100);
    Serial.println("CAREC speaker test — 1500 Hz beep on every LED-OFF transition");
    if (!speaker_test_init()) {
        Serial.println("[SPEAKER_TEST] init failed, halting");
        for (;;) vTaskDelay(pdMS_TO_TICKS(1000));
    }
    speaker_test_run();  // never returns
}

#elif defined(LED_TEST)

extern "C" void app_main() {
    initArduino();
    Serial.begin(115200);
    delay(100);
    Serial.println("CAREC LED test — WS2813 RGB on GPIO 40, 25-color cycle, 2 s ON / 58 s OFF");
    if (!led_test_init()) {
        Serial.println("[LED_TEST] init failed, halting");
        for (;;) vTaskDelay(pdMS_TO_TICKS(1000));
    }
    led_test_run();  // never returns
}

#elif defined(DISPLAY_TEST)

// PCA9535 LCD power-on (I2C0: SDA=GPIO47, SCL=GPIO48, addr=0x21)
// EXP_PWR_SYSTEM (bit2) must come on first, then all startup rails including
// EXP_PWR_LCD (bit1) — otherwise the SPD2010 panel stays dark.
//
// CRITICAL: drive all LCD signal pins LOW *before* the rail powers up, or the
// SPD2010 sees floating CS/CLK/data lines during boot and locks into an
// unrecoverable state. Mirrors `pulldown for lcd i2c` block in Seeed's
// bsp_i2c_bus_init().
static void _pca9535_power_lcd() {
    // 1. Drive LCD signal pins LOW as outputs BEFORE powering the rail
    gpio_config_t io_init = {};
    io_init.pin_bit_mask = (1ULL << GPIO_NUM_45)   // CS
                         | (1ULL << GPIO_NUM_8)    // BL
                         | (1ULL << GPIO_NUM_7)    // CLK
                         | (1ULL << GPIO_NUM_9)    // D0
                         | (1ULL << GPIO_NUM_1)    // D1
                         | (1ULL << GPIO_NUM_14)   // D2
                         | (1ULL << GPIO_NUM_13);  // D3
    io_init.mode         = GPIO_MODE_OUTPUT;
    io_init.pull_up_en   = GPIO_PULLUP_DISABLE;
    io_init.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_init.intr_type    = GPIO_INTR_DISABLE;
    gpio_config(&io_init);
    gpio_set_level(GPIO_NUM_45, 0);
    gpio_set_level(GPIO_NUM_8,  0);
    gpio_set_level(GPIO_NUM_7,  0);
    gpio_set_level(GPIO_NUM_9,  0);
    gpio_set_level(GPIO_NUM_1,  0);
    gpio_set_level(GPIO_NUM_14, 0);
    gpio_set_level(GPIO_NUM_13, 0);

    // 2. PCA9535 power-up sequence
    Wire.begin(47, 48, 400000UL);
    Wire.setTimeOut(20);
    Wire.beginTransmission(0x21); Wire.write(0x07); Wire.write(0x20); Wire.endTransmission();
    Wire.beginTransmission(0x21); Wire.write(0x03); Wire.write(0x04); Wire.endTransmission();
    delay(100);
    Wire.beginTransmission(0x21); Wire.write(0x03); Wire.write(0xDF); Wire.endTransmission();
    delay(200);
    Serial.println("[Power] PCA9535 LCD rail ON (pins pre-driven LOW)");
}

extern "C" void app_main() {
    initArduino();
    Serial.begin(115200);
    Serial.println("CAREC display test — RED 2s → YELLOW 2s → GREEN 2s → repeat");
    _pca9535_power_lcd();
    display_init();

    static const int zones[] = { ZONE_RED, ZONE_YELLOW, ZONE_GREEN };
    static const char* names[] = { "RED", "YELLOW", "GREEN" };
    int idx = 0;
    for (;;) {
        Serial.printf("[Test] zone → %s\n", names[idx]);
        display_set_zone(zones[idx]);
        idx = (idx + 1) % 3;
        // drive blink updates for 2 seconds
        unsigned long t = millis();
        while (millis() - t < 2000) {
            display_update();
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

#else  // ── Normal CAREC firmware below ──────────────────────────────────────

static int zone_to_beep(int zone) {
    if (zone == ZONE_RED)    return BEEP_CRITICAL;
    if (zone == ZONE_YELLOW) return BEEP_WARNING;
    return BEEP_NONE;
}

// WiFi credentials — copy firmware/config/wifi_config.h.template → wifi_config.h
// and fill in real credentials. File is gitignored.
#if __has_include("../config/wifi_config.h")
    #include "../config/wifi_config.h"
#else
    #define WIFI_SSID     "YOUR_2_4GHz_SSID"
    #define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#endif

static void wifi_connect() {
    Serial.printf("[WiFi] Connecting to %s", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) {
        delay(500);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
        Serial.printf("\n[WiFi] Connected: %s\n", WiFi.localIP().toString().c_str());
    else
        Serial.println("\n[WiFi] Not connected — OTA disabled, detection continues");
}

static void carec_setup() {
    Serial.begin(115200);
    Serial.println("CAREC v1.0 starting...");

    led_status_init();   // WS2813 RGB indicator on GPIO 40 (zone color/blink)
    sensecraft_init();   // SSCMA via SPI2 (Himax HX6538)
    display_init();      // QSPI SPD2010 412×412 (currently dark — debug in progress)
    beep_init();         // ES8311 codec + I2S0
    motion_init();       // always-forward stub (Phase 1)
    ble_logger_init();   // BLE GATT Nordic UART Service
    wifi_connect();      // WiFi for OTA (non-blocking on fail)

    Serial.println("CAREC ready.");
}

static void carec_loop() {
    // 1. Detect obstacles via Himax SSCMA
    DetectionResult result = sensecraft_detect();
    log_detections(&result);
    log_distances(&result);

    // 2–3. Estimate distance and classify. A detector failure is different
    // from a healthy frame containing no obstacles: failure enters the
    // conservative RED state and blocks OTA.
    float measured_dist_cm = nearest_obstacle_cm(&result);
    SafetyDecision decision = decide_safety_zone(result.status, measured_dist_cm);
    float dist_cm = decision.distance_cm;
    int zone = decision.zone;
    if (decision.detector_degraded) {
        Serial.printf("[Safety] detector unavailable (status=%d) → RED fail-safe\n",
                      static_cast<int>(result.status));
    }

    Serial.printf("[Safety] %.1f cm → %s\n", dist_cm,
        zone == ZONE_RED    ? "RED (<60cm)"       :
        zone == ZONE_YELLOW ? "YELLOW (60-100cm)"  : "GREEN (>100cm)");

    // 4. Speaker
    beep_trigger(zone_to_beep(zone));
    beep_update();

    // 5. Display + LED indicator
    display_set_zone(zone);
    display_update();
    led_status_set_zone(zone);
    led_status_update();

    // 6. BLE notification to caregiver app
    ble_log_event(dist_cm, zone, MOTION_FORWARD);

    // 7. OTA — only when clear path (no obstacle interruption risk)
    ota_set_safe(zone == ZONE_GREEN);
    ota_check_and_update();

    vTaskDelay(pdMS_TO_TICKS(100));
}

extern "C" void app_main() {
    initArduino();
    carec_setup();
    for (;;) {
        carec_loop();
    }
}

#endif  // SPEAKER_TEST / LED_TEST / DISPLAY_TEST / full firmware
