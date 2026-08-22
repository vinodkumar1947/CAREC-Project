# Hardware Constraints — SenseCAP Watcher W1-A Pin Map

## Reserved Pins — Never Reassign

| GPIO | Assignment | Reason |
|------|-----------|--------|
| **47** | PCA9535 I2C SDA | IO expander — power sequencing, AI chip, LCD, codec |
| **48** | PCA9535 I2C SCL | IO expander — same as above |
| **40** | WS2813 RGB LED | Onboard status LED (RMT channel) |

Do not suggest GPIO 47 or GPIO 48 for any user-facing feature. Do not suggest GPIO 40 for anything other than the LED strip.

## Full Pin Map (W1-A BSP)

### LCD — SPD2010 QSPI (SPI3_HOST)
| Signal | GPIO |
|--------|------|
| CLK    | 7    |
| D0     | 9    |
| D1     | 1    |
| D2     | 14   |
| D3     | 13   |
| CS     | 45   |
| BL     | 8 (LEDC PWM) |

### SSCMA / Himax HX6538 — SPI2_HOST (FSPI)
| Signal | GPIO |
|--------|------|
| SCK    | 4    |
| MISO   | 6    |
| MOSI   | 5    |
| CS     | 21   |

### Audio — ES8311 Codec
| Signal | GPIO |
|--------|------|
| I2S MCLK | 10 |
| I2S BCLK | 11 |
| I2S LRCK | 12 |
| I2S DOUT | 16 |
| I2C SDA (shared) | 47 |
| I2C SCL (shared) | 48 |
| I2C addr | 0x18 |

### Touch — CHSC6x
| Signal | GPIO |
|--------|------|
| SDA    | 39   |
| SCL    | 38   |

### PCA9535 IO Expander
| Signal | GPIO / Value |
|--------|-------------|
| SDA    | 47           |
| SCL    | 48           |
| Addr   | 0x21         |

## USB Ports (macOS)
- **`/dev/cu.usbmodem5B142665543`** — higher number — **ESP32-S3 flash/monitor**
- **`/dev/cu.usbmodem5B142665541`** — lower number — Himax WE2 console (do not flash here)

## PCA9535 Power Rails
Power sequencing must follow BSP order: SYSTEM → delay 100ms → all startup rails (SDCARD, LCD, AI_CHIP, CODEC_PA, GROVE, BAT_ADC) → delay 200ms → RST toggle on AI_CHIP. Never power rails out of order.
