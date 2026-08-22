# DIY WHEELCHAIR SAFETY PROJECTS - GITHUB COLLECTION
## Open-Source References for CAREC Development

**Last Updated:** April 27, 2026  
**Purpose:** Reference implementations you can learn from and adapt for CAREC

---

## 🏆 TOP RECOMMENDED PROJECTS (Start Here)

### 1. YOLOv8-BASED AUTONOMOUS WHEELCHAIR
**GitHub:** https://github.com/AgungHari/Development-of-YOLOV8-based-Autonomous-Wheelchair-for-Obstacle-Avoidance

**What it does:**
- Real-time object detection using YOLOv8
- Grid-based obstacle mapping (10x10 grid with OpenCV)
- Dual detection: Bounding box + pose landmarks
- ESP32 motor control integration
- Human detection + avoidance
- Achieves 81.85% mAP, 100% avoidance success rate

**Key Features for CAREC:**
- ✅ TinyML approach (can run on ESP32)
- ✅ YOLOv8-nano quantized model (lightweight)
- ✅ ESP32 control integration (exactly what you need)
- ✅ Grid mapping for path planning
- ✅ Multi-detection (humans, obstacles, terrain)

**Tech Stack:**
- Python (main code)
- YOLOv8 (object detection)
- MediaPipe (pose detection)
- OpenCV (image processing)
- NVIDIA CUDA (GPU acceleration, optional)
- ESP32 (motor control)

**Learn From:**
- How to integrate ML model with microcontroller
- Grid-based obstacle representation
- Real-time inference on edge device

**GitHub Stars:** ⭐⭐⭐⭐⭐ (Active, well-documented)

---

### 2. WODA PAPER - DEEP LEARNING OBSTACLE DETECTION
**GitHub:** https://github.com/yahyatawil/WODA_Paper

**What it does:**
- MobileNetv2 SSD fine-tuned for wheelchair obstacles
- EdgeImpulse integration (no-code ML training)
- Image-based control law (avoid obstacles to sides)
- Python-based implementation
- Raspberry Pi 4 + Camera + Sabertooth motor control

**Key Features for CAREC:**
- ✅ Uses EdgeImpulse (easier than raw TensorFlow)
- ✅ Custom dataset creation (campus sidewalks)
- ✅ Control law that maps detection to motor commands
- ✅ Lightweight model (MobileNet-based)
- ✅ Real-time performance on RPi

**Tech Stack:**
- Python (main code)
- EdgeImpulse (model training)
- OpenCV (visualization)
- Raspberry Pi 4 (compute)
- Sabertooth motor driver

**Learn From:**
- EdgeImpulse workflow (can train ML without coding)
- Control law: how to map obstacles to speed/direction
- Custom dataset creation process
- Published paper (academic validation)

**Paper:** "Deep Learning Obstacle Detection and Avoidance for Powered Wheelchair" (2022)

**GitHub Stars:** ⭐⭐⭐⭐ (Academic, peer-reviewed)

---

### 3. SMIFTY - SMART WHEELCHAIR WITH YOLO
**GitHub:** https://github.com/TeamAMPTY/Smart-Wheelchair

**What it does:**
- YOLO-based object detection
- Real-time obstacle avoidance
- Computer vision navigation
- Safety belt integration
- Emergency kill switch
- Autonomous movement with user override

**Key Features for CAREC:**
- ✅ Complete system (not just detection)
- ✅ Safety mechanisms (kill switch, belt)
- ✅ Shared control model (user + autonomy)
- ✅ Multi-object detection
- ✅ Production mindset

**Tech Stack:**
- Python (main code)
- YOLOv5/v8 (object detection)
- ROS (robotics framework, optional)

**Learn From:**
- Safety-first design
- Emergency stop systems
- Shared control paradigm (not full autonomy)

**GitHub Stars:** ⭐⭐⭐⭐ (Well-structured)

---

### 4. GESTURE-BASED WHEELCHAIR WITH OBSTACLE AVOIDANCE
**GitHub:** https://github.com/shaolink8/Gesture-Based-Smart-Wheel-Chair-with-Obstacle-Avoidance

**What it does:**
- Hand gesture recognition (cerebral palsy support)
- 3x ultrasonic sensors (left, center, right)
- Microbit + radio communication
- Automatic emergency stop on obstacle detection
- Voice memo system (memory support feature)

**Key Features for CAREC:**
- ✅ Multi-sensor ultrasonic setup (exactly what you plan)
- ✅ Distance threshold logic (simple but effective)
- ✅ Microbit programming (simpler than Arduino for beginners)
- ✅ Gesture control (alternative to joystick)
- ✅ Voice integration (nice addition)

**Tech Stack:**
- JavaScript (Microbit)
- Python (voice memos)
- Microbit boards
- 3x HC-SR04 ultrasonic sensors

**Learn From:**
- Simple ultrasonic sensor thresholding
- Multi-sensor coordination
- Gesture-based control (alternative interface)
- Accessible design for different disabilities

**GitHub Stars:** ⭐⭐⭐⭐ (Accessible design focus)

---

### 5. SMART WHEELS - TARGET FOLLOWING + AVOIDANCE
**GitHub:** https://github.com/ysshah/SmartWheels

**What it does:**
- Target-following autonomous wheelchair
- Real-time obstacle avoidance
- Path planning integration
- Vision-based control

**Key Features for CAREC:**
- ✅ Path planning algorithms
- ✅ Shared control (user input + autonomy)
- ✅ Real-time adjustment

**Tech Stack:**
- ROS (robotics framework)
- Python

**Learn From:**
- Path planning for tight spaces
- How to balance user control with autonomy

**GitHub Stars:** ⭐⭐⭐ (Moderate activity)

---

## 📚 SUPPLEMENTARY PROJECTS

### 6. IOT-ENABLED WHEELCHAIR (Head Gesture + Ultrasonic)
**GitHub:** https://github.com/im-gozmit/IoT-enabled-WheelChair

**Features:**
- Head gesture control
- Ultrasonic obstacle detection
- Remote control
- Pulse detection (health monitoring)
- GPS coordinates
- Bluetooth mobile app (planned)

**Learn From:**
- Health monitoring integration
- Head tracking sensors
- IoT connectivity patterns

---

### 7. WHEELCHAIR RETRIEVAL ROBOT
**GitHub:** https://github.com/Snak3Cheater/Wheelchair-Retrieval

**Features:**
- Wheelchair localization
- TensorFlow object detection
- Hand gesture recognition
- Ultrasonic mapping
- Autonomous navigation
- Retractable hooks for chair retrieval

**Learn From:**
- Complete autonomous pipeline
- Obstacle mapping with ultrasonic
- Multi-mode operation (search, explore, target)
- Machine learning for complex scenes

**Tech Stack:**
- Python
- TensorFlow
- Raspberry Pi
- Ultrasonic sensors

---

### 8. ESP ROBOT CONTROL (General Framework)
**GitHub:** https://github.com/lily-osp/esp-robot-control

**Features:**
- Web-based control interface
- WiFi connectivity
- Obstacle avoidance with ultrasonic
- Motor control (forward, backward, turn)
- Speed adjustment
- mDNS support

**Learn From:**
- WiFi control patterns
- Web interface design
- GPIO configuration for ESP32/ESP8266
- Motor driver integration (L298N)

**Tech Stack:**
- Arduino IDE
- ESP8266/ESP32
- HTML/WebServer
- Ultrasonic sensors

**GitHub Stars:** ⭐⭐⭐⭐ (Practical, well-documented)

---

### 9. ESP32-CAM ROBOT CAR
**GitHub:** https://github.com/SamRepository/EPS32Cam_RobotCar

**Features:**
- ESP32-CAM with obstacle detection
- Real-time video stream
- Web app for remote control
- Camera-based navigation
- Urban infrastructure monitoring

**Learn From:**
- Camera integration with ESP32
- Web UI design
- Real-time monitoring

**Tech Stack:**
- ESP32-CAM
- HTML/JavaScript
- Arduino IDE

---

### 10. WHEELCHAIR DS MOTION (ROS-Based)
**GitHub:** https://github.com/epfl-lasa/wheelchair-ds-motion

**Features:**
- Dynamical systems-based motion planning
- Obstacle avoidance using DS approach
- Gazebo simulation
- ROS integration
- Physically consistent Bayesian model

**Learn From:**
- Advanced motion planning algorithms
- ROS architecture for wheelchairs
- Simulation before hardware

**Tech Stack:**
- ROS (Robot Operating System)
- Gazebo (simulation)
- C++

**Note:** More academic/research-focused

---

### 11. IRIS CONTROL WHEELCHAIR (Eye Tracking)
**GitHub:** https://github.com/arjunsengupta98/Smart-wheelchair-for-paralysis-patients-using-Iris-control

**Features:**
- Eye movement detection
- Iris tracking via OpenCV
- Left/right/forward commands
- Custom detection algorithm
- Emergency braking on obstacle

**Learn From:**
- Eye tracking algorithms
- Custom OpenCV pipelines
- Assistive technology for paralysis patients
- Low-level computer vision

**Tech Stack:**
- Python
- OpenCV
- Raspberry Pi
- USB camera

---

### 12. REACTIVE ASSISTANCE (ROS Package)
**GitHub:** https://github.com/mazrk7/reactive_assistance

**Features:**
- Shared control obstacle avoidance
- Gap detection and navigation
- Autonomous goal-based movement
- Used for wheelchair research (IROS 2019 paper)
- Reactive motion planning

**Learn From:**
- Shared control theory
- Academic wheelchair research
- ROS package structure
- Explainability in shared control

**Tech Stack:**
- ROS
- Python/C++

---

## 🛠️ LEARNING RESOURCES (Non-Wheelchair Specific)

### ESP32 + Arduino IDE Resources

**Random Nerd Tutorials - ESP32 Projects:**  
https://randomnerdtutorials.com/projects-esp32/
- 250+ ESP32 tutorials
- Step-by-step instructions
- Schematics + code + videos
- Includes: WiFi, BLE, sensor integration, OTA updates

**ESP32 Infrared Obstacle Sensor:**  
https://esp32io.com/tutorials/esp32-infrared-obstacle-avoidance-sensor
- Simple IR sensor integration
- Complete Arduino code
- Serial output examples
- Good starting point for basic obstacle detection

---

## 📊 COMPARISON: Which Project to Learn From?

| Project | Best For | Complexity | Wheelchair-Ready? | Code Quality |
|---------|----------|-----------|------------------|-------------|
| **YOLOv8 Autonomous** | ML integration | ⭐⭐⭐⭐ | ✅ Yes (ESP32) | ⭐⭐⭐⭐⭐ |
| **WODA Paper** | EdgeImpulse workflow | ⭐⭐⭐ | ✅ Yes (Rpi) | ⭐⭐⭐⭐ |
| **SMIFTY** | Full system design | ⭐⭐⭐⭐ | ✅ Yes | ⭐⭐⭐⭐ |
| **Gesture Chair** | Simple sensors | ⭐⭐ | ✅ Yes | ⭐⭐⭐ |
| **Smart Wheels** | Advanced planning | ⭐⭐⭐⭐⭐ | ✅ Yes (ROS) | ⭐⭐⭐⭐ |
| **IoT Chair** | Multi-feature | ⭐⭐⭐ | ✅ Yes | ⭐⭐⭐ |
| **Wheelchair Retrieval** | Autonomous pipeline | ⭐⭐⭐⭐⭐ | ⚠️ Complex | ⭐⭐⭐⭐ |
| **ESP Robot** | WiFi control | ⭐⭐⭐ | ✅ Yes | ⭐⭐⭐⭐ |
| **ESP32-CAM** | Camera integration | ⭐⭐⭐ | ✅ Yes | ⭐⭐⭐ |
| **Iris Control** | Accessibility | ⭐⭐⭐ | ✅ Yes | ⭐⭐⭐ |

---

## 🎯 RECOMMENDED LEARNING PATH FOR CAREC

### Phase 1: Basic Sensors (Week 1-2)
1. Start: **Gesture-Based Chair** (simple ultrasonic)
2. Then: **ESP32 IR Sensor tutorial** (basic Arduino patterns)
3. Reference: **Random Nerd Tutorials** (general ESP32 knowledge)

### Phase 2: WiFi + Control (Week 2-3)
1. Study: **ESP Robot Control** (WiFi + web UI)
2. Add: OTA update patterns from **Random Nerd Tutorials**
3. Implement: Basic speed control

### Phase 3: Object Detection (Week 3-4)
1. Choose path A or B:
   - **Path A (Lightweight):** Gesture-Based + IR + simple thresholding
   - **Path B (Advanced):** YOLOv8-Autonomous + EdgeImpulse
2. Study: **WODA Paper** (control law design)

### Phase 4: Integration (Week 4-6)
1. Combine: Sensors + WiFi + ML (whichever level you chose)
2. Reference: **SMIFTY** (complete system structure)
3. Safety layer: **Iris Control** (emergency systems)

---

## 🔗 DIRECT GITHUB LINKS (Copy-Paste Ready)

```
ESSENTIAL:
https://github.com/AgungHari/Development-of-YOLOV8-based-Autonomous-Wheelchair-for-Obstacle-Avoidance
https://github.com/yahyatawil/WODA_Paper
https://github.com/shaolink8/Gesture-Based-Smart-Wheel-Chair-with-Obstacle-Avoidance
https://github.com/lily-osp/esp-robot-control

ADDITIONAL:
https://github.com/TeamAMPTY/Smart-Wheelchair
https://github.com/ysshah/SmartWheels
https://github.com/im-gozmit/IoT-enabled-WheelChair
https://github.com/SamRepository/EPS32Cam_RobotCar
https://github.com/arjunsengupta98/Smart-wheelchair-for-paralysis-patients-using-Iris-control
https://github.com/Snak3Cheater/Wheelchair-Retrieval
https://github.com/mazrk7/reactive_assistance
https://github.com/epfl-lasa/wheelchair-ds-motion

LEARNING RESOURCES:
https://github.com/topics/wheelchair-control (All wheelchair projects)
https://github.com/topics/esp32 (ESP32 projects)
https://github.com/espressif/arduino-esp32 (Official Arduino-ESP32)
https://randomnerdtutorials.com/projects-esp32/ (Tutorials)
https://esp32io.com/tutorials (Sensor tutorials)
```

---

## 💡 KEY TAKEAWAYS FOR CAREC

### From YOLOv8 Project:
- TinyML models CAN run on ESP32 with quantization
- Grid-based mapping is more efficient than continuous detection
- Pose landmarks + bounding box = better accuracy
- Consider using lightweight models (nano, not full size)

### From WODA Paper:
- EdgeImpulse simplifies ML training (no code needed)
- Control law: Map detection → motor commands (simple math)
- Custom datasets matter more than generic models
- Validation on actual terrain is critical

### From Gesture Chair:
- Simple ultrasonic thresholding is effective
- Multi-sensor fusion (left, center, right) gives good coverage
- Distance thresholds = configurable safety margins
- Emergency stop is essential

### From ESP Robot:
- WiFi control + web UI is straightforward with ESP32
- OTA updates work well (critical for CAREC)
- mDNS allows hostname-based access (better UX)
- Speed control via PWM is simple but effective

### From SMIFTY:
- Safety features (kill switch, belt) are non-negotiable
- Shared control is better than full autonomy for kids
- Multi-object detection supports multiple threat types
- User experience matters (responsiveness, feedback)

---

## 🚀 NEXT STEP: Choose Your Approach

**Option A: Start Simple (Gesture Chair + ESP Robot)**
- Level: Beginner
- Time: 2-3 weeks to working prototype
- Skills needed: Basic Arduino, WiFi basics
- Result: Ultrasonic detection + WiFi control

**Option B: Add ML (YOLOv8 + EdgeImpulse)**
- Level: Intermediate
- Time: 4-6 weeks to working prototype
- Skills needed: Python, basic ML, ESP32 C++
- Result: Object detection + smart avoidance

**Option C: Full Integration (All Above + Safety)**
- Level: Advanced
- Time: 8-12 weeks to production-ready
- Skills needed: Full stack (hardware, firmware, ML, UI)
- Result: Enterprise-grade safety system

---

**YOUR CHOICE:** Start with A, add B in Phase 2, polish with safety from C.

This is exactly your trajectory based on timeline + your expertise level.

---

**End of Reference Document**

*All projects are open source. Consider contributing back fixes/improvements!*
