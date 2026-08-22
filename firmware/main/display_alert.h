#pragma once
#include <Arduino.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <driver/spi_master.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include "esp_lcd_spd2010.h"
#include "../config/zone_config.h"

// Display alert driver — SenseCAP Watcher W1-A 1.45" SPD2010 (412×412, QSPI)
//
// Panel: SPD2010 (not ST7789). QSPI SPI mode 3.
// Pins confirmed from BSP sensecap-watcher.h:
//   CLK  GPIO7   BSP_LCD_PCLK
//   D0   GPIO9   BSP_LCD_DATA0
//   D1   GPIO1   BSP_LCD_DATA1
//   D2   GPIO14  BSP_LCD_DATA2
//   D3   GPIO13  BSP_LCD_DATA3
//   CS   GPIO45  BSP_LCD_SPI_CS
//   BL   GPIO8   BSP_LCD_GPIO_BL   (backlight, active-high)
//   RST  NC      BSP_LCD_GPIO_RST  (not connected)
//
// SPD2010 constraint: x_start and x_end must both be divisible by 4.
// LCD_W=412 satisfies this; partial-row writes must pad to 4-px boundary.
//
// Zone → color mapping:
//   ZONE_GREEN  → solid green  (#00FF00, 16-bit 0x07E0)
//   ZONE_YELLOW → yellow blink (DISPLAY_BLINK_YELLOW_MS half-period)
//   ZONE_RED    → red blink    (DISPLAY_BLINK_RED_MS half-period)

// ── Pin defines ───────────────────────────────────────────────────────────────

#define LCD_CLK  GPIO_NUM_7
#define LCD_D0   GPIO_NUM_9
#define LCD_D1   GPIO_NUM_1
#define LCD_D2   GPIO_NUM_14
#define LCD_D3   GPIO_NUM_13
#define LCD_CS   GPIO_NUM_45
#define LCD_BL   GPIO_NUM_8
#define LCD_W    412
#define LCD_H    412

// RGB565 colors
#define LCD_COLOR_RED    0xF800
#define LCD_COLOR_YELLOW 0xFFE0
#define LCD_COLOR_GREEN  0x07E0
#define LCD_COLOR_BLACK  0x0000

// ── State ─────────────────────────────────────────────────────────────────────

static esp_lcd_panel_handle_t _lcd_panel  = nullptr;
static bool                   _lcd_ready  = false;
static int                    _disp_zone  = -1;
static bool                   _disp_on    = false;
static unsigned long          _disp_last  = 0;

// Row buffer for efficient fill (one scanline = 412 × 2 bytes = 824 bytes)
static uint16_t _lcd_row[LCD_W];

// ── Internal fill ─────────────────────────────────────────────────────────────

static void _lcd_fill(uint16_t rgb565) {
    if (!_lcd_ready) {
        Serial.printf("[Display] fill skipped — not ready\n");
        return;
    }
    // SPD2010 via ESP-IDF receives big-endian RGB565; swap bytes
    uint16_t c = (rgb565 >> 8) | (rgb565 << 8);
    for (int i = 0; i < LCD_W; i++) _lcd_row[i] = c;
    for (int y = 0; y < LCD_H; y++) {
        esp_lcd_panel_draw_bitmap(_lcd_panel, 0, y, LCD_W, y + 1, _lcd_row);
    }
}

// ── Zone draw helpers ─────────────────────────────────────────────────────────

static void _draw_green()  { _lcd_fill(LCD_COLOR_GREEN);  }
static void _draw_yellow() { _lcd_fill(LCD_COLOR_YELLOW); }
static void _draw_red()    { _lcd_fill(LCD_COLOR_RED);    }
static void _draw_off()    { _lcd_fill(LCD_COLOR_BLACK);  }

// ── Public API ────────────────────────────────────────────────────────────────

inline void display_init() {
    // ── Backlight: LEDC PWM at 5 kHz, 10-bit duty (matches Seeed BSP) ───────
    // The W1-A backlight driver IC requires a PWM gate signal — a static
    // GPIO HIGH leaves the boost converter disabled and the panel dark.
    ledc_timer_config_t bl_timer = {};
    bl_timer.speed_mode      = LEDC_LOW_SPEED_MODE;
    bl_timer.duty_resolution = LEDC_TIMER_10_BIT;
    bl_timer.timer_num       = LEDC_TIMER_1;
    bl_timer.freq_hz         = 5000;
    bl_timer.clk_cfg         = LEDC_AUTO_CLK;
    ledc_timer_config(&bl_timer);

    ledc_channel_config_t bl_ch = {};
    bl_ch.gpio_num   = LCD_BL;
    bl_ch.speed_mode = LEDC_LOW_SPEED_MODE;
    bl_ch.channel    = LEDC_CHANNEL_1;
    bl_ch.timer_sel  = LEDC_TIMER_1;
    bl_ch.duty       = 0;                  // off during init
    bl_ch.hpoint     = 0;
    ledc_channel_config(&bl_ch);

    // ── QSPI bus (SPI3_HOST) — no SPICOMMON_BUSFLAG_QUAD; Seeed BSP doesn't set it
    spi_bus_config_t bus = {};
    bus.data0_io_num     = LCD_D0;
    bus.data1_io_num     = LCD_D1;
    bus.data2_io_num     = LCD_D2;
    bus.data3_io_num     = LCD_D3;
    bus.sclk_io_num      = LCD_CLK;
    bus.max_transfer_sz  = LCD_W * LCD_H * sizeof(uint16_t);
    esp_err_t err = spi_bus_initialize(SPI3_HOST, &bus, SPI_DMA_CH_AUTO);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        Serial.printf("[Display] SPI3 bus init failed: %s\n", esp_err_to_name(err));
        return;
    }

    // Panel IO
    esp_lcd_panel_io_handle_t io;
    esp_lcd_panel_io_spi_config_t io_cfg = {};
    io_cfg.cs_gpio_num       = LCD_CS;
    io_cfg.dc_gpio_num       = -1;       // embedded in QSPI protocol
    io_cfg.spi_mode          = 3;        // SPD2010 requires SPI mode 3
    io_cfg.pclk_hz           = 40 * 1000 * 1000;
    io_cfg.trans_queue_depth = 10;
    io_cfg.lcd_cmd_bits      = 32;
    io_cfg.lcd_param_bits    = 8;
    io_cfg.flags.quad_mode   = 1;
    if (esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI3_HOST, &io_cfg, &io) != ESP_OK) {
        Serial.println("[Display] Panel IO init failed");
        return;
    }

    // SPD2010 panel (QSPI interface)
    spd2010_vendor_config_t vendor_cfg = {};
    vendor_cfg.flags.use_qspi_interface = 1;

    esp_lcd_panel_dev_config_t panel_cfg = {};
    panel_cfg.reset_gpio_num   = -1;
    panel_cfg.rgb_ele_order    = LCD_RGB_ELEMENT_ORDER_RGB;
    panel_cfg.bits_per_pixel   = 16;
    panel_cfg.vendor_config    = &vendor_cfg;
    if (esp_lcd_new_panel_spd2010(io, &panel_cfg, &_lcd_panel) != ESP_OK) {
        Serial.println("[Display] SPD2010 panel init failed");
        return;
    }

    esp_lcd_panel_reset(_lcd_panel);
    esp_lcd_panel_init(_lcd_panel);
    esp_lcd_panel_mirror(_lcd_panel, false, false);  // matches Seeed BSP defaults
    esp_lcd_panel_disp_on_off(_lcd_panel, true);

    _lcd_ready = true;
    _draw_green();       // boot state: GREEN (safe)

    // Backlight on: ~80% duty (819 / 1024 at 10-bit). PWM was running at 0 during init.
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 819);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

    _disp_last = millis();
    Serial.println("[Display] SPD2010 QSPI 412x412 OK (BL via LEDC PWM @ 5 kHz, 80%)");
}

inline void display_set_zone(int zone) {
    if (zone == _disp_zone) return;
    _disp_zone = zone;
    _disp_on   = true;
    _disp_last = millis();

    switch (zone) {
        case ZONE_RED:    _draw_red();    break;
        case ZONE_YELLOW: _draw_yellow(); break;
        default:          _draw_green();  break;
    }
}

inline void display_update() {
    if (_disp_zone == ZONE_GREEN) return;   // solid, no blink needed

    uint16_t half_ms = (_disp_zone == ZONE_RED) ? DISPLAY_BLINK_RED_MS
                                                 : DISPLAY_BLINK_YELLOW_MS;
    unsigned long now = millis();
    if (now - _disp_last < half_ms) return;

    _disp_last = now;
    _disp_on   = !_disp_on;

    if (_disp_on) {
        switch (_disp_zone) {
            case ZONE_RED:    _draw_red();    break;
            case ZONE_YELLOW: _draw_yellow(); break;
            default:          break;
        }
    } else {
        _draw_off();
    }
}
