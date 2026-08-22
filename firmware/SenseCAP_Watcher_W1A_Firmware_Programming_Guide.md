# SenseCAP Watcher W1A - Complete Firmware Programming Guide

## Overview
This guide covers everything you need to program custom firmware to your SenseCAP Watcher W1A device, including hardware connections, required tools, and step-by-step flashing procedures.

---

## 1. HARDWARE CONNECTIONS

### USB Connection Requirements
**⚠️ CRITICAL**: The Watcher has TWO USB ports - only ONE supports data transfer.

| Port | Location | Function |
|------|----------|----------|
| **Bottom/Side USB-C** | Side of device | ✅ **Data Transfer & Power** (USE THIS ONE) |
| **Back USB-C** | Back of device | ⚠️ Power Only (NO data) |

### Connection Steps
1. Use a proper **USB-C data cable** (not just a charging cable)
2. Connect the **bottom/side USB-C port** to your computer
3. When properly connected, you'll see:
   - 1 USB device entry
   - 2 UART serial ports (one for ESP32S3, one for Himax)

### Identifying Serial Ports
**macOS Example:**
```
/dev/tty.wchusbserial56F3067xxxx  (first UART)
/dev/tty.wchusbserial56F3067yyyy  (second UART)
```

**Windows Example:**
```
COM3, COM4, COM5 (check Device Manager)
```

**Linux Example:**
```
/dev/ttyACM0, /dev/ttyACM1
```

---

## 2. REQUIRED TOOLS & SOFTWARE

### A. ESP-IDF (Espressif IoT Development Framework)

**Version Required:** v5.2.1 (exact version for factory firmware)

#### Installation on macOS/Linux:
```bash
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout v5.2.1
./install.sh
```

#### Create IDF Alias (Recommended):
Add to your `~/.bash_profile`, `~/.bashrc`, or `~/.zshrc`:
```bash
alias get_idf='. ~/esp/esp-idf/export.sh'
```

Then reload shell:
```bash
source ~/.bash_profile
```

### B. esptool.py
Automatically installed with ESP-IDF. Verify:
```bash
which esptool.py    # macOS/Linux
where esptool.py    # Windows
```

### C. Clone the Watcher Repository
```bash
git clone https://github.com/Seeed-Studio/SenseCAP-Watcher.git
cd SenseCAP-Watcher
git submodule update --init --recursive
```

### D. Optional: Visual Tools
- **SquareLine Studio** - For custom UI development
- **Visual Studio Code** - For code editing
- **Serial Monitor** - `idf.py monitor`, `screen`, or `pyserial miniterm`

---

## 3. STEP-BY-STEP FIRMWARE FLASHING

### Step 1: Setup IDF Environment
```bash
# Activate IDF environment
get_idf

# Verify IDF is working
idf.py

# You should see help output with usage information
```

### Step 2: Set Target Chip
```bash
idf.py set-target esp32s3
```

### Step 3: Navigate to Firmware Directory
```bash
cd SenseCAP-Watcher/example/factory_firmware
```

### Step 4: **BACKUP nvsfactory Partition** (⚠️ IMPORTANT)
This partition contains critical factory data (EUI, device info). **Never erase it!**

```bash
# Find your serial port first
ls /dev/tty.wchusbserial*     # macOS
ls /dev/ttyACM*                # Linux
# Check Device Manager         # Windows

# Backup the partition (replace port name)
esptool.py --port /dev/tty.wchusbserial56F3067xxxx \
  --baud 2000000 \
  --chip esp32s3 \
  --before default_reset \
  --after hard_reset \
  --no-stub read_flash 0x9000 204800 nvsfactory.bin
```

**Save `nvsfactory.bin` in a safe location!**

### Step 5: Build the Firmware
```bash
idf.py build
```

**Successful build output will show:**
```
[100%] Built target app
...
Project build complete.
```

### Step 6: Flash the Firmware

**Option A: Safe Flash (Recommended - preserves nvsfactory)**
```bash
idf.py --port /dev/tty.wchusbserial56F3067xxxx -b 2000000 app-flash
```

**Option B: Full Flash (for complete update)**
```bash
idf.py --port /dev/tty.wchusbserial56F3067xxxx -b 2000000 flash
```

**Flashing takes 1-2 minutes. You'll see:**
```
Compressed 1234KB of data...
Writing at 0x00010000...
[====      ] 45%
```

### Step 7: Monitor Serial Output
```bash
idf.py --port /dev/tty.wchusbserial56F3067xxxx monitor
```

**Exit monitoring:** Press `Ctrl + ]`

### Step 8: Restart the Device
Press and hold the wheel button for 3 seconds to power-cycle and load new firmware.

---

## 4. DEVICE CONNECTIONS DURING FLASHING

### Before Flashing
✅ Connect via **bottom/side USB-C** port only
✅ Ensure proper USB data cable
✅ Device should power on when connected
✅ Check Device Manager/System Report for serial ports

### During Flashing
- Do NOT disconnect USB cable
- Do NOT power off device
- Monitor progress in terminal
- Device will auto-reset after flashing

### After Flashing
- Device will restart automatically
- Watch serial monitor for boot logs
- Device will enter normal operation
- Use SenseCraft app to configure

---

## 5. TROUBLESHOOTING

### Serial Port Not Appearing
```bash
# macOS - List all tty devices
ls /dev/tty.*

# Linux
dmesg | grep tty

# Windows - Check Device Manager for COM ports
# Drivers may need CH340 drivers from:
# https://sparks.gogo.co.nz/ch340.html
```

### Port Already in Use
```bash
# Kill previous connections
# macOS/Linux
lsof /dev/tty.wchusbserial*
kill -9 <PID>

# Windows - Close all serial monitor instances
```

### Build Fails
```bash
# Clean and rebuild
idf.py fullclean
idf.py build
```

### Flash Fails with Permission Error (Linux)
```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER
# Then log out and log back in
```

### Device Won't Boot After Flash
1. Try flashing again
2. Check if nvsfactory was accidentally erased
3. Use `app-flash` instead of full flash next time
4. Restore from backup if needed

### Cannot Monitor Output
- Try the other serial port (2 UART ports available)
- Example: if `/dev/ttyACM0` doesn't work, try `/dev/ttyACM1`
- Check baud rate (typically 115200)

---

## 6. ADVANCED: FLASHING HIMAX AI CHIP

For dual-chip programming (Himax AI chip):

### Flashing Himax Firmware
```bash
# When connecting to both chips, identify correctly:
# ESP32S3 = port ending with "B"
# Himax = port ending with "A"

esptool.py --port /dev/tty.wchusbserial***A \
  -b 2000000 write_flash 0x0 himax_firmware.bin
```

---

## 7. USEFUL COMMANDS REFERENCE

```bash
# Enter IDF environment
get_idf

# Set target
idf.py set-target esp32s3

# Build firmware
idf.py build

# Flash app only (safe)
idf.py --port /dev/ttyACM0 -b 2000000 app-flash

# Monitor output
idf.py --port /dev/ttyACM0 monitor

# Build and flash together
idf.py --port /dev/ttyACM0 -b 2000000 build flash monitor

# Erase flash (WARNING - erases nvsfactory!)
esptool.py --port /dev/ttyACM0 erase_flash

# Read factory partition (backup)
esptool.py --port /dev/ttyACM0 read_flash 0x9000 204800 nvsfactory.bin

# List available examples
ls example/
```

---

## 8. IMPORTANT WARNINGS

⚠️ **Critical Precautions:**

1. **nvsfactory Partition**
   - Contains unique device EUI and factory data
   - NEVER erase accidentally
   - Always backup before flashing
   - Use `app-flash` to preserve it

2. **Correct USB Port**
   - Only bottom/side USB-C port supports data
   - Back USB port is power only
   - Verify you see 2 UART ports

3. **USB Cable**
   - Must be a proper USB data cable
   - Not all USB-C cables support data transfer
   - Test with file transfer if uncertain

4. **Power**
   - Keep device powered during flashing
   - USB connection provides power
   - Don't pull cable during operation

5. **IDF Version**
   - Factory firmware requires v5.2.1 exactly
   - Other versions may not work
   - Use `git checkout v5.2.1` if needed

---

## 9. POST-FLASHING SETUP

After successful firmware flashing:

1. **Power Cycle Device**
   - Hold wheel button 3 seconds
   - Let it boot completely

2. **Connect to SenseCraft App**
   - Download SenseCraft mobile app
   - Rotate wheel to show QR code
   - Scan QR with phone
   - Configure Wi-Fi (2.4GHz only)
   - Name your device

3. **Test Functionality**
   - Monitor serial output for errors
   - Check SenseCraft app connection
   - Run example tasks

4. **Custom Development**
   - Modify firmware in `example/` directories
   - Rebuild and test iteratively
   - Refer to Watcher Software Framework docs

---

## 10. HELPFUL RESOURCES

- **Official Wiki:** https://wiki.seeedstudio.com/watcher/
- **GitHub Repository:** https://github.com/Seeed-Studio/SenseCAP-Watcher
- **ESP-IDF Docs:** https://docs.espressif.com/projects/esp-idf/en/v5.2.1/esp32s3/
- **SenseCraft AI:** https://sensecraft.seeed.cc/ai/
- **Community Forum:** https://forum.seeedstudio.com/
- **Discord Support:** https://discord.gg/eWkprNDMU7

---

## 11. QUICK REFERENCE CHECKLIST

### Before You Start:
- [ ] Downloaded ESP-IDF v5.2.1
- [ ] Created `get_idf` alias
- [ ] Cloned SenseCAP-Watcher repository
- [ ] Have proper USB data cable
- [ ] Have serial monitor software
- [ ] Located correct USB port (bottom/side)

### Flashing Process:
- [ ] Activated IDF environment (`get_idf`)
- [ ] Set target to esp32s3
- [ ] Backed up nvsfactory partition
- [ ] Built firmware successfully
- [ ] Flashed with `app-flash` (safe method)
- [ ] No disconnections during flash

### After Flashing:
- [ ] Device powered on and booted
- [ ] Serial monitor shows logs
- [ ] Connected to SenseCraft app
- [ ] Wi-Fi configured
- [ ] Test task running successfully

---

**Last Updated:** May 2026
**Based on:** Seeed Studio Wiki & Official Documentation
**Device:** SenseCAP Watcher W1A
**Firmware Version:** Factory Firmware (ESP-IDF v5.2.1)
