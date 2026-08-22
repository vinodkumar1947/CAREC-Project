#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <driver/i2s_std.h>
#include <driver/gpio.h>
#include <math.h>
#include "../config/zone_config.h"

// Directional beep patterns — SenseCAP Watcher W1-A 1W speaker
//
// Speaker path: ESP32-S3 → I2S0 → ES8311 codec → 1W amplifier → speaker
//
// Audio hardware (from BSP sensecap-watcher.h):
//   I2S port  : I2S_NUM_0
//   MCLK      : GPIO10  (256 × 16 kHz = 4.096 MHz)
//   BCLK      : GPIO11
//   LRCK      : GPIO12
//   DOUT      : GPIO16  (DAC output to ES8311)
//
// ES8311 codec (from BSP):
//   I2C bus   : I2C_NUM_0, SDA=GPIO47, SCL=GPIO48, 400 kHz
//   I2C addr  : 0x18 (7-bit) = 0x30 (8-bit as in BSP DRV_ES8311_I2C_ADDR)
//
// Beep patterns (timing from zone_config.h):
//   BEEP_WARNING  : 1 beep/sec, 1500 Hz (YELLOW zone)
//   BEEP_CRITICAL : 3-pulse burst, 2000 Hz (RED zone)
//   BEEP_NONE     : silence
//
// Implementation: PCM sine wave written via I2S in 10 ms chunks each loop().
// The I2S DMA ring buffer (640 ms capacity) absorbs timing jitter from the
// detection loop. Non-blocking — write() returns immediately if DMA is full.

// ── I2C / I2S pin constants ───────────────────────────────────────────────────

#define AUDIO_I2C_SDA  GPIO_NUM_47
#define AUDIO_I2C_SCL  GPIO_NUM_48
#define AUDIO_I2C_ADDR 0x18   // ES8311 7-bit addr (BSP: 0x30 >> 1)

#define AUDIO_MCLK     GPIO_NUM_10
#define AUDIO_BCLK     GPIO_NUM_11
#define AUDIO_LRCK     GPIO_NUM_12
#define AUDIO_DOUT     GPIO_NUM_16

#define AUDIO_SAMPLE_RATE 16000
#define AUDIO_CHUNK_MS    10   // samples written per loop() call

// ── Pattern IDs ───────────────────────────────────────────────────────────────

#define BEEP_NONE      -1
#define BEEP_WARNING    0
#define BEEP_CRITICAL   1

// ── State ─────────────────────────────────────────────────────────────────────

static i2s_chan_handle_t _i2s_tx       = nullptr;
static bool              _audio_ready  = false;

static int           _beep_pattern    = BEEP_NONE;
static int           _beep_step       = 0;
static bool          _beep_tone_on    = false;
static unsigned long _beep_step_start = 0;
static uint32_t      _sine_sample_idx = 0;   // continuous phase counter

// ── ES8311 register write ─────────────────────────────────────────────────────

static void _es8311_write(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(AUDIO_I2C_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

// ── ES8311 init + DAC start — 16 kHz / 16-bit / mono / slave / MCLK=4.096 MHz
// Sequence derived verbatim from espressif/esp-adf's es8311_codec_init() and
// es8311_start(ES_MODULE_DAC). Both passes are required; previously we only
// did init (no DAC power-up + no route to speaker), so the codec was silent.

static bool _es8311_init() {
    Wire.begin((int)AUDIO_I2C_SDA, (int)AUDIO_I2C_SCL);
    Wire.setClock(400000);

    // Probe: verify ES8311 responds at 0x18
    Wire.beginTransmission(AUDIO_I2C_ADDR);
    if (Wire.endTransmission() != 0) {
        Serial.println("[Audio] ES8311 not found at I2C 0x18 — check SDA/SCL GPIO47/48");
        return false;
    }

    // ── Init phase (matches esp-adf es8311_codec_init) ──────────────────────
    // GPIO noise immunity — first I2C write to ES8311 occasionally fails, so twice.
    _es8311_write(0x44, 0x08);
    _es8311_write(0x44, 0x08);

    // Clock manager
    _es8311_write(0x01, 0x30);  // MCLK on, CLK on
    _es8311_write(0x02, 0x00);  // ADC/DAC divider high byte
    _es8311_write(0x03, 0x10);  // ADC_DIV=1, ADC_OSR=32
    _es8311_write(0x16, 0x24);  // ADC HPF (always written, even DAC-only)
    _es8311_write(0x04, 0x10);  // DAC_DIV=1
    _es8311_write(0x05, 0x00);
    _es8311_write(0x06, 0x03);  // BCLK_DIV: MCLK/8 = 512 kHz
    _es8311_write(0x07, 0x00);  // LRCK MSB
    _es8311_write(0x08, 0x1F);  // LRCK LSB → BCLK/32 = 16 kHz

    // Serial data port — I2S standard, 16-bit
    _es8311_write(0x09, 0x00);  // SDP_IN  — I2S 16-bit (DAC mute bit cleared in start)
    _es8311_write(0x0A, 0x00);  // SDP_OUT — I2S 16-bit

    // System registers (BIAS, RESET-DONE, SLAVE)
    _es8311_write(0x0B, 0x00);
    _es8311_write(0x0C, 0x00);
    _es8311_write(0x10, 0x1F);  // SYSTEM REG10 — bias config (was 0x03 = WRONG)
    _es8311_write(0x11, 0x7F);  // SYSTEM REG11 — bias config (was UNWRITTEN)
    _es8311_write(0x00, 0x80);  // CSM start, slave mode (bit 7 = "reset done")
    _es8311_write(0x01, 0x3F);  // all clocks enabled

    // ── Start phase, DAC mode (matches esp-adf es8311_start) ────────────────
    _es8311_write(0x13, 0x10);  // SYSTEM REG13
    _es8311_write(0x1B, 0x0A);  // ADC default
    _es8311_write(0x1C, 0x6A);  // ADC equalizer

    // DAC power-up — without these the analog path stays off → silence
    _es8311_write(0x0E, 0x02);  // Power up analog DAC bias (was 0xFF = WRONG)
    _es8311_write(0x12, 0x00);  // DAC power up (clear power-down bits) ← KEY
    _es8311_write(0x14, 0x1A);  // Analog playback select
    _es8311_write(0x0D, 0x01);  // DAC analog block on
    _es8311_write(0x15, 0x40);  // Route DAC to speaker output ← KEY
    _es8311_write(0x37, 0x08);  // DAC ramp rate

    // GPIO_REG44 — internal reference signal (ADCL + DACR)
    _es8311_write(0x44, 0x58);

    // DAC volume + unmute (last so any prior writes don't clip)
    _es8311_write(0x32, 0xBF);  // DAC volume = 0 dB (was 0x00 = mute)
    _es8311_write(0x31, 0x00);  // DAC unmuted (was 0x60 = mute bits set)

    Serial.println("[Audio] ES8311 init OK (16 kHz / 16-bit / mono / DAC → SPK)");
    return true;
}

// ── I2S init ──────────────────────────────────────────────────────────────────

static bool _i2s_init() {
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    chan_cfg.auto_clear = true;
    if (i2s_new_channel(&chan_cfg, &_i2s_tx, nullptr) != ESP_OK) {
        Serial.println("[Audio] I2S channel create failed");
        return false;
    }

    i2s_std_config_t std_cfg = {};
    std_cfg.clk_cfg.sample_rate_hz     = AUDIO_SAMPLE_RATE;
    std_cfg.clk_cfg.clk_src            = I2S_CLK_SRC_DEFAULT;
    std_cfg.clk_cfg.mclk_multiple      = I2S_MCLK_MULTIPLE_256;  // 256×16k=4.096MHz

    std_cfg.slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(
        I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO);

    std_cfg.gpio_cfg.mclk         = AUDIO_MCLK;
    std_cfg.gpio_cfg.bclk         = AUDIO_BCLK;
    std_cfg.gpio_cfg.ws           = AUDIO_LRCK;
    std_cfg.gpio_cfg.dout         = AUDIO_DOUT;
    std_cfg.gpio_cfg.din          = I2S_GPIO_UNUSED;
    std_cfg.gpio_cfg.invert_flags.mclk_inv = false;
    std_cfg.gpio_cfg.invert_flags.bclk_inv = false;
    std_cfg.gpio_cfg.invert_flags.ws_inv   = false;

    if (i2s_channel_init_std_mode(_i2s_tx, &std_cfg) != ESP_OK) {
        Serial.println("[Audio] I2S std mode init failed");
        return false;
    }
    if (i2s_channel_enable(_i2s_tx) != ESP_OK) {
        Serial.println("[Audio] I2S channel enable failed");
        return false;
    }

    Serial.println("[Audio] I2S0 OK (MCLK=GPIO10, BCLK=GPIO11, LRCK=GPIO12, DOUT=GPIO16)");
    return true;
}

// ── Sine wave PCM generator ───────────────────────────────────────────────────

static const int CHUNK_SAMPLES = (AUDIO_SAMPLE_RATE * AUDIO_CHUNK_MS) / 1000;  // 160

static void _write_tone(uint16_t freq_hz) {
    int16_t buf[CHUNK_SAMPLES];
    for (int i = 0; i < CHUNK_SAMPLES; i++) {
        float phase = (float)(_sine_sample_idx % AUDIO_SAMPLE_RATE) * freq_hz
                      / (float)AUDIO_SAMPLE_RATE;
        buf[i] = (int16_t)(sinf(phase * 2.0f * (float)M_PI) * 20000.0f);
        _sine_sample_idx++;
    }
    size_t written;
    i2s_channel_write(_i2s_tx, buf, sizeof(buf), &written, 0);  // non-blocking
}

static void _write_silence() {
    static int16_t silence[CHUNK_SAMPLES] = {};
    size_t written;
    i2s_channel_write(_i2s_tx, silence, sizeof(silence), &written, 0);
    _sine_sample_idx = 0;
}

// ── Beep step table (mirrors zone_config.h constants) ────────────────────────

struct BeepStep { uint16_t freq_hz; uint16_t on_ms; uint16_t off_ms; };

static const BeepStep PATTERN_WARNING[] = {
    {1500, BEEP_WARNING_PULSE_MS,
           BEEP_WARNING_PERIOD_MS - BEEP_WARNING_PULSE_MS},
};
static const BeepStep PATTERN_CRITICAL[] = {
    {2000, BEEP_CRITICAL_PULSE_MS, BEEP_CRITICAL_GAP_MS},
    {2000, BEEP_CRITICAL_PULSE_MS, BEEP_CRITICAL_GAP_MS},
    {2000, BEEP_CRITICAL_PULSE_MS, BEEP_CRITICAL_PERIOD_MS},
};

static int _nsteps(int pat) {
    if (pat == BEEP_WARNING)  return 1;
    if (pat == BEEP_CRITICAL) return BEEP_CRITICAL_PULSES;
    return 0;
}
static const BeepStep* _steps(int pat) {
    if (pat == BEEP_WARNING)  return PATTERN_WARNING;
    if (pat == BEEP_CRITICAL) return PATTERN_CRITICAL;
    return nullptr;
}

// ── Public API ────────────────────────────────────────────────────────────────

inline void beep_init() {
    bool codec_ok = _es8311_init();
    bool i2s_ok   = _i2s_init();
    _audio_ready  = codec_ok && i2s_ok;
    if (!_audio_ready)
        Serial.println("[Audio] Degraded: detection continues, no beep");
}

// Low-level public helpers — write one 10 ms chunk of tone or silence.
// Useful for SPEAKER_TEST and other diagnostics that bypass the pattern state
// machine. The pattern state machine still owns beep_update() in normal mode.
inline bool beep_ready() { return _audio_ready; }
inline void beep_play_tone(uint16_t freq_hz) { if (_audio_ready) _write_tone(freq_hz); }
inline void beep_play_silence()              { if (_audio_ready) _write_silence(); }

inline void beep_trigger(int pattern) {
    if (pattern == _beep_pattern) return;
    _beep_pattern    = pattern;
    _beep_step       = 0;
    _beep_tone_on    = true;   // start each new pattern with the tone on
    _beep_step_start = millis();
}

// Drive the beep state machine + write one 10 ms audio chunk. Call every loop().
inline void beep_update() {
    if (!_audio_ready) return;

    if (_beep_pattern == BEEP_NONE) {
        _write_silence();
        return;
    }

    const BeepStep* steps  = _steps(_beep_pattern);
    int             nsteps = _nsteps(_beep_pattern);
    if (!steps || nsteps == 0) { _write_silence(); return; }

    const BeepStep&  s       = steps[_beep_step];
    unsigned long    now     = millis();
    unsigned long    elapsed = now - _beep_step_start;

    if (_beep_tone_on) {
        if (elapsed < s.on_ms) {
            _write_tone(s.freq_hz);
        } else {
            // Tone phase complete → enter silence phase
            _beep_tone_on    = false;
            _beep_step_start = now;
            _write_silence();
        }
    } else {
        if (elapsed >= s.off_ms) {
            // Silence phase complete → advance step and start next tone
            _beep_step       = (_beep_step + 1) % nsteps;
            _beep_step_start = now;
            _beep_tone_on    = true;
        }
        _write_silence();
    }
}
