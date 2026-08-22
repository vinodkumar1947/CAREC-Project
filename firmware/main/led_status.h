#pragma once
#include <Arduino.h>
#include <esp_err.h>
#include "led_strip.h"
#include "../config/zone_config.h"

// CAREC safety-zone status indicator on the onboard WS2813 RGB LED (GPIO 40).
//
// Mirrors the display_alert.h zone semantics so the LED is a parallel
// visible indicator while the SPD2010 LCD is offline.
//
//   ZONE_GREEN  → solid green
//   ZONE_YELLOW → yellow blink (DISPLAY_BLINK_YELLOW_MS full period)
//   ZONE_RED    → red blink    (DISPLAY_BLINK_RED_MS full period)
//
// Hardware: WS2813 mini on GPIO 40, 3.3 V always-on rail (no PCA9535 gating).
// The same pixel that rgb_led_test.h drives during LED_TEST mode.

#ifndef LED_STATUS_GPIO
#define LED_STATUS_GPIO         40
#endif
#ifndef LED_STATUS_BRIGHTNESS
#define LED_STATUS_BRIGHTNESS   80      // 0..255; scales palette before refresh
#endif

static led_strip_handle_t _led_status_strip = nullptr;
static int           _led_status_zone = -1;
static bool          _led_status_on   = false;
static unsigned long _led_status_last = 0;

static inline void _led_status_write(uint8_t r, uint8_t g, uint8_t b) {
    if (!_led_status_strip) return;
    const uint8_t rr = (uint16_t(r) * LED_STATUS_BRIGHTNESS) / 255;
    const uint8_t gg = (uint16_t(g) * LED_STATUS_BRIGHTNESS) / 255;
    const uint8_t bb = (uint16_t(b) * LED_STATUS_BRIGHTNESS) / 255;
    led_strip_set_pixel(_led_status_strip, 0, rr, gg, bb);
    led_strip_refresh(_led_status_strip);
}

static inline void _led_status_off() {
    if (_led_status_strip) led_strip_clear(_led_status_strip);
}

static inline bool led_status_init() {
    led_strip_config_t cfg = {};
    cfg.strip_gpio_num = LED_STATUS_GPIO;
    cfg.max_leds = 1;
    led_strip_rmt_config_t rmt = {};
    rmt.resolution_hz = 10 * 1000 * 1000;
    rmt.flags.with_dma = false;
    esp_err_t err = led_strip_new_rmt_device(&cfg, &rmt, &_led_status_strip);
    if (err != ESP_OK) {
        Serial.printf("[LED] init failed: %s\n", esp_err_to_name(err));
        return false;
    }
    _led_status_off();
    Serial.printf("[LED] zone indicator on GPIO %d, brightness %d/255\n",
                  LED_STATUS_GPIO, LED_STATUS_BRIGHTNESS);
    return true;
}

// Switch to a new zone. Solid colors apply immediately; blink zones start ON.
static inline void led_status_set_zone(int zone) {
    if (zone == _led_status_zone) return;
    _led_status_zone = zone;
    _led_status_on   = true;
    _led_status_last = millis();
    switch (zone) {
        case ZONE_GREEN:  _led_status_write(  0, 255,   0); break;
        case ZONE_YELLOW: _led_status_write(255, 255,   0); break;
        case ZONE_RED:    _led_status_write(255,   0,   0); break;
        default:          _led_status_off();                break;
    }
}

// Call periodically (e.g. from carec_loop). Handles blink phase for RED/YELLOW.
static inline void led_status_update() {
    if (!_led_status_strip) return;
    unsigned long period_ms = 0;
    uint8_t r = 0, g = 0, b = 0;
    switch (_led_status_zone) {
        case ZONE_YELLOW: period_ms = DISPLAY_BLINK_YELLOW_MS; r = 255; g = 255; b = 0; break;
        case ZONE_RED:    period_ms = DISPLAY_BLINK_RED_MS;    r = 255; g = 0;   b = 0; break;
        default: return;  // GREEN is solid; UNKNOWN is off
    }
    const unsigned long half = period_ms / 2;
    const unsigned long now = millis();
    if (now - _led_status_last >= half) {
        _led_status_on   = !_led_status_on;
        _led_status_last = now;
        if (_led_status_on) _led_status_write(r, g, b);
        else                _led_status_off();
    }
}
