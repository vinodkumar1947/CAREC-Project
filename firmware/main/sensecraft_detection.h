#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Seeed_Arduino_SSCMA.h>
#include "safety_decision.h"

// SenseCraft object detection — SenseCAP Watcher W1-A
//
// The Himax HX6538 (WE2) runs AI inference on its own firmware.
// The ESP32-S3 reads bounding-box results via the SSCMA client over SPI2.
//
// SPI2 pins (BSP sensecap-watcher.h, BSP_SSCMA_CLIENT_SPI_NUM = SPI2_HOST):
//   SCK  GPIO4   BSP_SPI2_HOST_SCLK
//   MISO GPIO6   BSP_SPI2_HOST_MISO
//   MOSI GPIO5   BSP_SPI2_HOST_MOSI
//   CS   GPIO21  BSP_SSCMA_CLIENT_SPI_CS
//   CLK  12 MHz  BSP_SSCMA_CLIENT_SPI_CLK
//   SYNC -1      IO_EXPANDER_PIN_NUM_6 (polling mode, no direct GPIO)
//   RST  -1      IO_EXPANDER_PIN_NUM_7 (toggled manually below)
//
// Arduino FSPI=1=SPI2_HOST — display uses SPI3_HOST (HSPI=2), no conflict.
//
// PCA9535 16-bit IO expander (BSP: esp_io_expander_pca95xx_16bit):
//   I2C0: SDA=GPIO47, SCL=GPIO48, address=0x21 (PCA9535_ADDRESS_001)
//   PIN_10 = BSP_PWR_SYSTEM   (port 1 bit 2) — MUST be HIGH before AI chip
//   PIN_11 = BSP_PWR_AI_CHIP  (port 1 bit 3) — MUST be HIGH to power Himax
//   PIN_7  = BSP_SSCMA_CLIENT_RST (port 0 bit 7) — active-low reset
//
// Boot sequence (mirrors sensecap-watcher.c bsp_board_init):
//   all outputs = 0 → SYSTEM = 1 → delay 100ms →
//   all startup = 1 (AI_CHIP included) → delay 200ms →
//   RST: low 50ms → high → delay 800ms for WE2 SSCMA firmware boot
//
// Box coordinate scale: SSCMA returns pixel coordinates in the model's input
// image space. Divide by SSCMA_SCALE to get 0–1 normalised values used by
// distance_estimator.h. The WE2 default COCO model runs at 640px wide input,
// matching CAMERA_INFERENCE_W_PX in distance_estimator.h.
//
// CALIBRATE on first run: stand a person at exactly 60 cm. Open Serial Monitor
// (115200 baud). The "[SSCMA Calibrate]" output prints raw w values. bbox_w
// should be ~0.241 normalised (raw w ≈ 154 at 640px scale). If off, adjust
// SSCMA_SCALE to match the actual model input width.

#define SSCMA_SCK    4
#define SSCMA_MISO   6
#define SSCMA_MOSI   5
#define SSCMA_CS    21
#define SSCMA_CLK   (12 * 1000 * 1000)   // BSP_SSCMA_CLIENT_SPI_CLK

#define SSCMA_SCALE 640.0f   // WE2 model input width in pixels → normalised divisor

// PCA9535 IO expander constants
#define PCA9535_ADDR     0x21   // ESP_IO_EXPANDER_I2C_PCA9535_ADDRESS_001
#define PCA9535_I2C_SDA  47     // BSP_GENERAL_I2C_SDA
#define PCA9535_I2C_SCL  48     // BSP_GENERAL_I2C_SCL
#define PCA9535_REG_OUT0 0x02   // Output port 0 (pins 0–7)
#define PCA9535_REG_OUT1 0x03   // Output port 1 (pins 8–15)
#define PCA9535_REG_CFG0 0x06   // Config port 0 (0=output, 1=input)
#define PCA9535_REG_CFG1 0x07   // Config port 1 (0=output, 1=input)

// Power bits in port 1 (pin N → bit N-8)
#define EXP_PWR_SDCARD   (1 << 0)  // PIN_8
#define EXP_PWR_LCD      (1 << 1)  // PIN_9
#define EXP_PWR_SYSTEM   (1 << 2)  // PIN_10 BSP_PWR_SYSTEM
#define EXP_PWR_AI_CHIP  (1 << 3)  // PIN_11 BSP_PWR_AI_CHIP
#define EXP_PWR_CODEC_PA (1 << 4)  // PIN_12
#define EXP_PWR_GROVE    (1 << 6)  // PIN_14
#define EXP_PWR_BAT_ADC  (1 << 7)  // PIN_15
// Bit 5 (PIN_13 = BSP_PWR_BAT_DET) is an input — not driven

#define EXP_PWR_STARTUP  (EXP_PWR_SDCARD | EXP_PWR_LCD | EXP_PWR_SYSTEM | \
                          EXP_PWR_AI_CHIP | EXP_PWR_CODEC_PA | \
                          EXP_PWR_GROVE   | EXP_PWR_BAT_ADC)

// ── Detection structs ─────────────────────────────────────────────────────────

struct Detection {
    char  label[32];
    float confidence;    // 0.0–1.0
    float bbox_x;        // normalised centre-x (0–1)
    float bbox_y;        // normalised centre-y (0–1)
    float bbox_w;        // normalised width    (0–1) — used for distance
    float bbox_h;        // normalised height   (0–1)
};

#define MAX_DETECTIONS 8

struct DetectionResult {
    Detection items[MAX_DETECTIONS];
    int       count;
    uint32_t  inference_ms;
    DetectionStatus status;
};

#define DETECTION_CONFIDENCE_THRESHOLD 0.55f

// ── State ─────────────────────────────────────────────────────────────────────

static SPIClass _sscma_spi(FSPI);   // FSPI=1=SPI2_HOST — display is on SPI3_HOST, no conflict
static SSCMA    _ai;
static bool     _ai_ready         = false;
static bool     _sscma_first_det  = true;
static int      _invoke_count     = 0;

// ── COCO class ID → label string ─────────────────────────────────────────────
// Standard COCO labels used by SenseCraft default YOLOv5/v8 model.
// Add entries as you encounter new class IDs in Serial output.

static const char* _coco_label(uint8_t id) {
    switch (id) {
        // People & animals
        case  0: return "person";
        case 15: return "cat";
        case 16: return "dog";
        // Vehicles (hallway / ramp situations)
        case  1: return "bicycle";
        case  2: return "car";
        case  3: return "motorcycle";
        // Furniture — primary wheelchair obstacles
        case 13: return "bench";
        case 56: return "chair";
        case 57: return "couch";
        case 58: return "plant";
        case 59: return "bed";
        case 60: return "table";
        // Bags / items on floor
        case 24: return "backpack";
        case 26: return "handbag";
        case 28: return "suitcase";
        // Electronics
        case 63: return "laptop";
        case 64: return "mouse";
        case 67: return "phone";
        case 72: return "tv";
        // Toys (child environment)
        case 32: return "ball";
        case 77: return "teddy";
        // Misc
        case 73: return "book";
        case 74: return "clock";
        default: return nullptr;
    }
}

// ── PCA9535 helper ────────────────────────────────────────────────────────────

static void _pca9535_write(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(PCA9535_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

// ── Himax WE2 power-on + reset via PCA9535 IO expander ───────────────────────
//
// Mirrors bsp_board_init() in sensecap-watcher.c:
//   1. Configure port 1: all outputs except bit 5 (PIN_13=BAT_DET input)
//   2. All outputs = 0 (power off)
//   3. SYSTEM power (PIN_10) = 1, wait 100ms
//   4. All startup power = 1 (includes AI_CHIP PIN_11), wait 200ms
//   5. RST on PIN_7: configure as output, drive LOW 50ms, HIGH, wait 800ms

static void _himax_power_and_reset() {
    Wire.begin(PCA9535_I2C_SDA, PCA9535_I2C_SCL, 400000UL);
    Wire.setTimeOut(20);

    // Port 1: all outputs except bit 5 (PIN_13 = BAT_DET, kept as input)
    _pca9535_write(PCA9535_REG_CFG1, 0x20);
    // Port 0: all inputs (detection signals) — RST reconfigured below
    _pca9535_write(PCA9535_REG_CFG0, 0xFF);

    // Step 1: all power off
    _pca9535_write(PCA9535_REG_OUT1, 0x00);

    // Step 2: SYSTEM power on first
    _pca9535_write(PCA9535_REG_OUT1, EXP_PWR_SYSTEM);
    delay(100);

    // Step 3: all startup power on — this powers the Himax WE2
    _pca9535_write(PCA9535_REG_OUT1, EXP_PWR_STARTUP);
    delay(200);   // extra margin beyond BSP's 50ms for AI chip to stabilise

    // Step 4: RST toggle on PIN_7 (port 0, bit 7)
    _pca9535_write(PCA9535_REG_CFG0, 0x7F);   // PIN_7 → output
    _pca9535_write(PCA9535_REG_OUT0, 0x00);    // RST low (active)
    delay(50);
    _pca9535_write(PCA9535_REG_OUT0, 0x80);    // RST high (released)
    delay(800);   // WE2 needs ~700ms to boot SSCMA firmware
}

// ── Public API ────────────────────────────────────────────────────────────────

static void sensecraft_init() {
    _himax_power_and_reset();

    // FSPI = SPI2_HOST — confirmed BSP_SSCMA_CLIENT_SPI_NUM = SPI2_HOST
    _sscma_spi.begin(SSCMA_SCK, SSCMA_MISO, SSCMA_MOSI, -1);

    for (int attempt = 1; attempt <= 3; attempt++) {
        if (_ai.begin(&_sscma_spi, SSCMA_CS, -1, -1, SSCMA_CLK)) {
            _ai_ready = true;
            Serial.printf("[Detection] SSCMA OK (attempt %d) — Himax HX6538 via SPI2\n", attempt);
            Serial.printf("[Detection] Himax ID=%s  name=%s  info=%s\n",
                          _ai.ID(true) ? _ai.ID(true) : "?",
                          _ai.name(true) ? _ai.name(true) : "?",
                          _ai.info(false).length() ? _ai.info(false).c_str() : "?");
            return;
        }
        Serial.printf("[Detection] SSCMA attempt %d failed, retrying...\n", attempt);
        delay(500);
    }
    Serial.println("[Detection] SSCMA init FAILED — check PCA9535 I2C GPIO47/48 addr 0x21, SPI2 GPIO4/5/6/21");
}

static DetectionResult sensecraft_detect() {
    DetectionResult result;
    result.count        = 0;
    result.inference_ms = 0;
    result.status       = DETECTION_NOT_READY;

    if (!_ai_ready) return result;

    result.status = DETECTION_OK;

    // Arduino SSCMA invoke(times, filter, show) sends AT+INVOKE=<t>,<!f>,<f>.
    // The official sscma_client SDK sends AT+INVOKE=<t>,<f>,<noshow> which for
    // (filter=false,show=false) gives AT+INVOKE=1,0,1.
    // Calling invoke(1, true, false) through the Arduino library produces
    // AT+INVOKE=1,0,1 — matching the official SDK that the Watcher firmware expects.
    int inv_ret = _ai.invoke(1, true, false);

    result.inference_ms = _ai.perf().inference;
    _invoke_count++;

    // Diagnostic: log raw box counts (first 5 invocations + every 30)
    size_t raw_boxes = _ai.boxes().size();
    if (_invoke_count <= 5 || (_invoke_count % 30) == 0) {
        Serial.printf("[SSCMA Raw] invoke#%d ret=%d: %zu boxes, %ums inference\n",
                      _invoke_count, inv_ret, raw_boxes, (unsigned)_ai.perf().inference);
        for (size_t i = 0; i < raw_boxes && i < 4; i++) {
            const auto& b = _ai.boxes()[i];
            Serial.printf("  raw[%zu] class=%u score=%u x=%u y=%u w=%u h=%u\n",
                          i, b.target, b.score, b.x, b.y, b.w, b.h);
        }
    }

    if (inv_ret != 0 && raw_boxes == 0) {
        result.status = DETECTION_INFERENCE_FAILED;
        return result;
    }

    for (size_t i = 0; i < raw_boxes && result.count < MAX_DETECTIONS; i++) {
        const auto& b = _ai.boxes()[i];
        Detection&  d = result.items[result.count];

        d.confidence = b.score / 100.0f;
        if (d.confidence < DETECTION_CONFIDENCE_THRESHOLD) continue;

        d.bbox_x = b.x / SSCMA_SCALE;
        d.bbox_y = b.y / SSCMA_SCALE;
        d.bbox_w = b.w / SSCMA_SCALE;
        d.bbox_h = b.h / SSCMA_SCALE;

        const char* lbl = _coco_label(b.target);
        if (lbl) {
            strncpy(d.label, lbl, 31);
        } else {
            snprintf(d.label, 32, "class_%u", b.target);
        }
        d.label[31] = '\0';
        result.count++;
    }

    // One-time calibration dump on first detection
    if (_sscma_first_det && result.count > 0) {
        _sscma_first_det = false;
        Serial.println("[SSCMA Calibrate] Raw box dump — place person at 60 cm:");
        for (size_t i = 0; i < _ai.boxes().size(); i++) {
            const auto& b = _ai.boxes()[i];
            Serial.printf("  class=%u score=%u  x=%u y=%u w=%u h=%u  → bbox_w=%.3f\n",
                          b.target, b.score, b.x, b.y, b.w, b.h,
                          (float)b.w / SSCMA_SCALE);
        }
        Serial.println("[SSCMA Calibrate] Expected bbox_w ≈ 0.241 at 60 cm for a person.");
    }

    return result;
}

static void log_detections(const DetectionResult* r) {
    if (r->count == 0) return;
    Serial.printf("[Detection] %d obj(s) in %lums:\n", r->count, (unsigned long)r->inference_ms);
    for (int i = 0; i < r->count; i++) {
        const Detection* d = &r->items[i];
        Serial.printf("  [%d] %-10s conf=%.2f  bbox_w=%.3f\n",
                      i, d->label, d->confidence, d->bbox_w);
    }
}
