# W1-A Onboard RGB LED — Bring-up Notes

Confirms the SenseCAP Watcher W1-A onboard RGB LED works. Useful as a hardware sanity check and as the simplest "is the toolchain alive?" smoke test.

Two ways to run it:
- **In-tree (recommended):** define `LED_TEST` in [`main/CAREC_main.cpp`](main/CAREC_main.cpp) and flash the project as usual. The test source is [`main/rgb_led_test.h`](main/rgb_led_test.h), color palette and timing live there.
- **Standalone:** a copy of the IDF `get-started/blink` example in `/tmp/blink_carec` with `CONFIG_BLINK_GPIO=40`. Useful when you don't want to rebuild the whole CAREC firmware. Recipe in the [Standalone build & flash](#standalone-build--flash) section below.

## Hardware

- **LED:** WS2813 mini, single addressable pixel, WS2812-protocol compatible
- **GPIO:** 40 (data line)
- **Supply:** 3.3 V, always-on (no PCA9535 power gating required for the LED itself)
- **Source:** confirmed from Seeed reference firmware (`sensecap_watcher.cc` on Seeed Wiki, "RGB LED型号为 ws2813 mini, 连接在GPIO 40")

Pins to **avoid** on this board if you ever repurpose the example:
- GPIO 47 / 48 — I²C0 SDA/SCL to PCA9535 (default ESP-IDF blink uses GPIO 48; will not light the LED)
- GPIO 4 / 5 / 6 / 21 — SPI2 to Himax WE2 (SSCMA)
- GPIO 1 / 7 / 8 / 9 / 13 / 14 / 45 — SPD2010 LCD QSPI + backlight

## Toolchain workaround on this Mac

The system `python3` is 3.14, but the ESP-IDF venv is at `~/.espressif/python_env/idf5.3_py3.9_env/` (Python 3.9). Running `. ~/esp/esp-idf/export.sh` alone fails because it picks up 3.14. Workaround: prepend the 3.9 venv to `PATH` first, then call `idf.py` via its full path.

```sh
export PATH="$HOME/.espressif/python_env/idf5.3_py3.9_env/bin:$PATH"
. ~/esp/esp-idf/export.sh
python ~/esp/esp-idf/tools/idf.py <command>
```

## In-tree build & flash

1. Open [`firmware/main/CAREC_main.cpp`](main/CAREC_main.cpp), uncomment `#define LED_TEST` near the top, and comment out `#define DISPLAY_TEST`. Exactly one mode may be active at a time (a `#error` enforces this).
2. Build & flash from `firmware/`:
   ```sh
   export PATH="$HOME/.espressif/python_env/idf5.3_py3.9_env/bin:$PATH"
   . ~/esp/esp-idf/export.sh
   cd firmware
   python ~/esp/esp-idf/tools/idf.py -p /dev/cu.usbmodem5B142665543 flash monitor
   ```

Serial output (115200 baud, one entry per pulse):
```
CAREC LED test — WS2813 RGB on GPIO 40, 25-color cycle, 2 s ON / 58 s OFF
I (xxx) ... [LED_TEST] WS2812 init on GPIO 40, 25 colors, 2000 ms ON / 58000 ms OFF
I (xxx) ... [LED_TEST] [ 1/25] red          RGB= 80,  0,  0  ON 2000 ms
I (xxx) ... [LED_TEST]                                   OFF 58000 ms
```

To return to the normal CAREC firmware, comment out both test flags and reflash. To tune brightness or timing, edit the constants at the top of [`main/rgb_led_test.h`](main/rgb_led_test.h) (`LED_TEST_BRIGHTNESS`, `LED_TEST_ON_MS`, `LED_TEST_OFF_MS`).

## Standalone build & flash

For a quick toolchain-only smoke test that doesn't rebuild CAREC. Lives in a scratch dir outside the repo so it doesn't pollute the firmware build. Recreate it with:

```sh
cp -r ~/esp/esp-idf/examples/get-started/blink /tmp/blink_carec
```

Then edit `/tmp/blink_carec/sdkconfig.defaults.esp32s3`:

```
CONFIG_BLINK_LED_STRIP=y
CONFIG_BLINK_GPIO=40
CONFIG_BLINK_PERIOD=500
```

Replace `/tmp/blink_carec/main/blink_example_main.c` with the [color-cycle source](#color-cycle-source) below, then:

```sh
export PATH="$HOME/.espressif/python_env/idf5.3_py3.9_env/bin:$PATH"
. ~/esp/esp-idf/export.sh
cd /tmp/blink_carec
python ~/esp/esp-idf/tools/idf.py set-target esp32s3
python ~/esp/esp-idf/tools/idf.py build
python ~/esp/esp-idf/tools/idf.py -p /dev/cu.usbmodem5B142665543 flash
```

(Use the higher-numbered `usbmodem` port — that's the W1-A's IDF flash interface. The lower-numbered port is the Himax WE2 console.)

## Watching the serial log

Pick whichever you prefer. Only one program can hold the port at a time.

```sh
# 1. idf.py monitor — colored, decodes panics, Ctrl-T menu (exit Ctrl-])
python ~/esp/esp-idf/tools/idf.py -p /dev/cu.usbmodem5B142665543 monitor

# 2. screen — built into macOS (exit Ctrl-A K y)
screen /dev/cu.usbmodem5B142665543 115200

# 3. pyserial miniterm (exit Ctrl-])
$HOME/.espressif/python_env/idf5.3_py3.9_env/bin/python -m serial.tools.miniterm \
    /dev/cu.usbmodem5B142665543 115200
```

Hit the W1-A reset button after attaching the monitor to capture the boot banner.

## Behavior

- 24 distinct hues at 15° HSV spacing + white = **25 colors total**
- Each cycle: **2 s ON** showing one color, **58 s OFF**
- Color advances by one each cycle, wrapping every 25 minutes
- `LED_BRIGHTNESS = 80` (out of 255). Bump it if too dim, drop it if too bright.

Serial output per pulse looks like:

```
I (320) rgb_cycle: Init WS2812 on GPIO 40, 25 colors, 2000 ms ON / 58000 ms OFF
I (330) rgb_cycle: [ 1/25] red          RGB= 80,  0,  0  ON 2000 ms
I (2340) rgb_cycle:        OFF 58000 ms
```

## Color-cycle source

Drop into `/tmp/blink_carec/main/blink_example_main.c`:

```c
// CAREC W1-A RGB color-cycle test
// WS2813 mini RGB LED on GPIO 40. 2 s ON, 58 s OFF, advance one color each cycle.

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "rgb_cycle";

#define BLINK_GPIO       CONFIG_BLINK_GPIO
#define ON_MS            2000
#define OFF_MS           58000
#define LED_BRIGHTNESS   80

static led_strip_handle_t led_strip;

static const struct { uint8_t r, g, b; const char *name; } palette[] = {
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
static const size_t PALETTE_N = sizeof(palette) / sizeof(palette[0]);

static void configure_led(void) {
    ESP_LOGI(TAG, "Init WS2812 on GPIO %d, %d colors, %d ms ON / %d ms OFF",
             BLINK_GPIO, (int)PALETTE_N, ON_MS, OFF_MS);
    led_strip_config_t strip_config = { .strip_gpio_num = BLINK_GPIO, .max_leds = 1 };
    led_strip_rmt_config_t rmt_config = { .resolution_hz = 10 * 1000 * 1000, .flags.with_dma = false };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    led_strip_clear(led_strip);
}

void app_main(void) {
    configure_led();
    size_t idx = 0;
    while (1) {
        uint8_t r = (palette[idx].r * LED_BRIGHTNESS) / 255;
        uint8_t g = (palette[idx].g * LED_BRIGHTNESS) / 255;
        uint8_t b = (palette[idx].b * LED_BRIGHTNESS) / 255;
        ESP_LOGI(TAG, "[%2u/%u] %-11s  RGB=%3u,%3u,%3u  ON %d ms",
                 (unsigned)(idx + 1), (unsigned)PALETTE_N, palette[idx].name, r, g, b, ON_MS);
        led_strip_set_pixel(led_strip, 0, r, g, b);
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(ON_MS));

        led_strip_clear(led_strip);
        ESP_LOGI(TAG, "       OFF %d ms", OFF_MS);
        vTaskDelay(pdMS_TO_TICKS(OFF_MS));

        idx = (idx + 1) % PALETTE_N;
    }
}
```

## Re-flashing the real CAREC firmware afterwards

```sh
export PATH="$HOME/.espressif/python_env/idf5.3_py3.9_env/bin:$PATH"
. ~/esp/esp-idf/export.sh
cd firmware
python ~/esp/esp-idf/tools/idf.py -p /dev/cu.usbmodem5B142665543 app-flash
```
