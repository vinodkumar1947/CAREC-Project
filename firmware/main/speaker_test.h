#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "rgb_led_test.h"             // re-uses LED setup + palette + brightness
#include "directional_beep_patterns.h" // ES8311 + I²S driver (codec_init, i2s_init, tone helpers)

// Standalone speaker bring-up test for SenseCAP Watcher W1-A.
//
// Cycle (2 s):
//   1.5 s — LED ON  with next palette color, silence pumped to I²S
//   0.5 s — LED OFF + 1500 Hz tone (so the audible beep coincides with a visible OFF)
//
// Goal: prove the ES8311 codec + I²S0 + 1 W amplifier chain works in isolation
// from SSCMA / detection / WiFi / BLE. Once confirmed, the same beep_init()
// driver gets wired into the normal carec_loop().
//
// Enable by defining SPEAKER_TEST in CAREC_main.cpp. Mutually exclusive with
// LED_TEST and DISPLAY_TEST (each test mode owns app_main entirely).

#ifndef SPEAKER_TEST_TONE_HZ
#define SPEAKER_TEST_TONE_HZ   1500
#endif
#ifndef SPEAKER_TEST_LED_ON_MS
#define SPEAKER_TEST_LED_ON_MS 59500   // ~60 s heartbeat cycle
#endif
#ifndef SPEAKER_TEST_BEEP_MS
#define SPEAKER_TEST_BEEP_MS   500
#endif

// ── PCA9535 power-on ─────────────────────────────────────────────────────────
// Audio path needs EXP_PWR_CODEC (bit 4) on. SYSTEM (bit 2) must come up first.
// Same I²C bus (GPIO47/48 @ 0x21) the ES8311 sits on (0x18) — single Wire init.

static void _speaker_test_power_on() {
    Wire.begin(47, 48, 400000UL);
    Wire.setTimeOut(20);
    // Port 1 direction: all outputs except bit 5 (BAT_DET = input)
    Wire.beginTransmission(0x21); Wire.write(0x07); Wire.write(0x20); Wire.endTransmission();
    // SYSTEM rail first
    Wire.beginTransmission(0x21); Wire.write(0x03); Wire.write(0x04); Wire.endTransmission();
    delay(100);
    // All startup rails: LCD=1, SYSTEM=2, AI=3, CODEC=4, GROVE=6, BAT_ADC=7
    Wire.beginTransmission(0x21); Wire.write(0x03); Wire.write(0xDF); Wire.endTransmission();
    delay(200);
    Serial.println("[SPEAKER_TEST] PCA9535 SYSTEM + CODEC rails ON");
}

static inline bool speaker_test_init() {
    _speaker_test_power_on();
    if (!led_test_init()) {
        Serial.println("[SPEAKER_TEST] LED init failed");
        return false;
    }
    beep_init();
    if (!beep_ready()) {
        Serial.println("[SPEAKER_TEST] audio init failed (codec or I²S)");
        return false;
    }
    Serial.printf("[SPEAKER_TEST] ready — %d Hz tone, LED %d ms ON / OFF+beep %d ms\n",
                  SPEAKER_TEST_TONE_HZ, SPEAKER_TEST_LED_ON_MS, SPEAKER_TEST_BEEP_MS);
    return true;
}

[[noreturn]] static inline void speaker_test_run() {
    size_t idx = 0;
    for (;;) {
        // ── Phase A: LED on, silent ─────────────────────────────────────────
        const uint8_t r = (uint16_t(_led_test_palette[idx].r) * LED_TEST_BRIGHTNESS) / 255;
        const uint8_t g = (uint16_t(_led_test_palette[idx].g) * LED_TEST_BRIGHTNESS) / 255;
        const uint8_t b = (uint16_t(_led_test_palette[idx].b) * LED_TEST_BRIGHTNESS) / 255;
        Serial.printf("[SPEAKER_TEST] [%2u/%u] %-11s  LED ON %d ms (silent)\n",
                      (unsigned)(idx + 1), (unsigned)_LED_TEST_N,
                      _led_test_palette[idx].name, SPEAKER_TEST_LED_ON_MS);
        led_strip_set_pixel(_led_test_strip, 0, r, g, b);
        led_strip_refresh(_led_test_strip);

        unsigned long t = millis();
        while (millis() - t < SPEAKER_TEST_LED_ON_MS) {
            beep_play_silence();         // keep DMA fed
            vTaskDelay(pdMS_TO_TICKS(AUDIO_CHUNK_MS));
        }

        // ── Phase B: LED off + beep ─────────────────────────────────────────
        led_strip_clear(_led_test_strip);
        Serial.printf("[SPEAKER_TEST]              LED OFF + %d Hz beep %d ms\n",
                      SPEAKER_TEST_TONE_HZ, SPEAKER_TEST_BEEP_MS);
        t = millis();
        while (millis() - t < SPEAKER_TEST_BEEP_MS) {
            beep_play_tone(SPEAKER_TEST_TONE_HZ);
            vTaskDelay(pdMS_TO_TICKS(AUDIO_CHUNK_MS));
        }
        beep_play_silence();              // flush DMA after tone

        idx = (idx + 1) % _LED_TEST_N;
    }
}
