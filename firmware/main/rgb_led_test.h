#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include "led_strip.h"

// Standalone WS2813 RGB LED color-cycle test for SenseCAP Watcher W1-A.
//
// Hardware: single WS2813-mini RGB LED on GPIO 40 (WS2812-protocol compatible),
// 3.3 V supply. No PCA9535 power gating required for the LED.
//
// Behavior: cycles through 24 distinct HSV hues + white. Each cycle is
// LED_TEST_ON_MS ON / LED_TEST_OFF_MS OFF, advancing one color per cycle.
//
// Enable by defining LED_TEST in CAREC_main.cpp. Mutually exclusive with
// DISPLAY_TEST and the normal CAREC loop (each takes over app_main).
//
// Serial output (115200 baud) per cycle:
//   [LED_TEST] [ 1/25] red         RGB= 80,  0,  0  ON 2000 ms
//   [LED_TEST]                                       OFF 58000 ms

#define LED_TEST_GPIO          40
#define LED_TEST_ON_MS         2000
#define LED_TEST_OFF_MS        58000
#define LED_TEST_BRIGHTNESS    80    // 0..255; scales the palette before refresh

static const struct { uint8_t r, g, b; const char *name; } _led_test_palette[] = {
    {255,   0,   0, "red"        }, {255,  64,   0, "red-orange" },
    {255, 128,   0, "orange"     }, {255, 191,   0, "amber"      },
    {255, 255,   0, "yellow"     }, {191, 255,   0, "yellow-grn" },
    {128, 255,   0, "chartreuse" }, { 64, 255,   0, "lime"       },
    {  0, 255,   0, "green"      }, {  0, 255,  64, "spring"     },
    {  0, 255, 128, "mint"       }, {  0, 255, 191, "aqua"       },
    {  0, 255, 255, "cyan"       }, {  0, 191, 255, "sky"        },
    {  0, 128, 255, "azure"      }, {  0,  64, 255, "blue-cyan"  },
    {  0,   0, 255, "blue"       }, { 64,   0, 255, "indigo"     },
    {128,   0, 255, "violet"     }, {191,   0, 255, "purple"     },
    {255,   0, 255, "magenta"    }, {255,   0, 191, "pink-mag"   },
    {255,   0, 128, "pink"       }, {255,   0,  64, "rose"       },
    {255, 255, 255, "white"      },
};
static const size_t _LED_TEST_N = sizeof(_led_test_palette) / sizeof(_led_test_palette[0]);

static led_strip_handle_t _led_test_strip = nullptr;

static inline bool led_test_init() {
    led_strip_config_t cfg = {};
    cfg.strip_gpio_num = LED_TEST_GPIO;
    cfg.max_leds = 1;
    led_strip_rmt_config_t rmt = {};
    rmt.resolution_hz = 10 * 1000 * 1000;
    rmt.flags.with_dma = false;
    esp_err_t err = led_strip_new_rmt_device(&cfg, &rmt, &_led_test_strip);
    if (err != ESP_OK) {
        Serial.printf("[LED_TEST] init failed: %s\n", esp_err_to_name(err));
        return false;
    }
    led_strip_clear(_led_test_strip);
    Serial.printf("[LED_TEST] WS2812 init on GPIO %d, %u colors, %d ms ON / %d ms OFF\n",
                  LED_TEST_GPIO, (unsigned)_LED_TEST_N, LED_TEST_ON_MS, LED_TEST_OFF_MS);
    return true;
}

[[noreturn]] static inline void led_test_run() {
    size_t idx = 0;
    for (;;) {
        const uint8_t r = (_led_test_palette[idx].r * LED_TEST_BRIGHTNESS) / 255;
        const uint8_t g = (_led_test_palette[idx].g * LED_TEST_BRIGHTNESS) / 255;
        const uint8_t b = (_led_test_palette[idx].b * LED_TEST_BRIGHTNESS) / 255;
        Serial.printf("[LED_TEST] [%2u/%u] %-11s  RGB=%3u,%3u,%3u  ON %d ms\n",
                      (unsigned)(idx + 1), (unsigned)_LED_TEST_N,
                      _led_test_palette[idx].name, r, g, b, LED_TEST_ON_MS);
        led_strip_set_pixel(_led_test_strip, 0, r, g, b);
        led_strip_refresh(_led_test_strip);
        vTaskDelay(pdMS_TO_TICKS(LED_TEST_ON_MS));

        led_strip_clear(_led_test_strip);
        Serial.printf("[LED_TEST]                                   OFF %d ms\n", LED_TEST_OFF_MS);
        vTaskDelay(pdMS_TO_TICKS(LED_TEST_OFF_MS));

        idx = (idx + 1) % _LED_TEST_N;
    }
}
