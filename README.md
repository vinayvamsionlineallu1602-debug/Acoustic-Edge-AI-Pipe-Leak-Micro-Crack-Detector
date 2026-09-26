# Acoustic Edge-AI Pipe Leak & Micro-Crack Detector

A compact ESP32-based acoustic monitoring prototype designed to detect abnormal pipe sounds associated with **water leaks and micro-cracks**.

The system uses an **I2S MEMS microphone** to capture acoustic vibrations from a pipe, processes the signal on an **ESP32**, and provides immediate visual and audible alerts through an **OLED display and buzzer**.

> **Project Type:** IoT / Embedded Systems / Edge AI Prototype
> **Controller:** ESP32
> **Sensor:** I2S MEMS Microphone
> **Display:** 0.96" SSD1306 OLED
> **Alert:** Active Buzzer
> **Firmware:** Arduino C++
> **AI Extension:** TinyML / Edge Impulse

---

## 📌 Project Overview

Small pipe leaks and micro-cracks can produce weak acoustic signals that may be difficult to identify through manual inspection. Industrial environments can further complicate detection because pumps, machinery, and surrounding activity introduce background noise.

This project explores a compact monitoring system that captures acoustic signals directly from a pipe and analyzes them locally using an ESP32.

The prototype follows this general workflow:

```text
Pipe
 │
 │ Acoustic vibrations
 ▼
I2S MEMS Microphone
 │
 │ Digital audio
 ▼
ESP32
 │
 │ Signal processing
 ▼
Detection Logic
 │
 ├───────────────┐
 ▼               ▼
OLED Display    Buzzer
 │               │
 ▼               ▼
Status          Alert
```

The proposed Edge-AI version extends this architecture with a TinyML classifier running directly on the ESP32.

---

## 🎯 Objectives

The main objectives of the project are:

1. Detect acoustic patterns associated with pipe leaks and defects.
2. Capture pipe vibrations using an I2S MEMS microphone.
3. Process acoustic signals using an ESP32.
4. Provide real-time detection without requiring constant cloud connectivity.
5. Display the detected condition and signal information on an OLED.
6. Provide an audible warning when abnormal activity is detected.
7. Develop a compact and portable prototype.
8. Provide a foundation for future TinyML-based acoustic classification.

---

## ✨ Features

* Real-time acoustic monitoring
* ESP32-based processing
* Digital I2S microphone interface
* RMS-based acoustic signal measurement
* Leak detection threshold
* Micro-crack detection threshold
* Consecutive-sample confirmation to reduce false alerts
* OLED status display
* Audible buzzer alerts
* Serial Monitor diagnostics
* Portable embedded-system architecture
* Extendable to TinyML / Edge Impulse

---

## 🧰 Hardware Requirements

| Component                   | Purpose                                 |
| --------------------------- | --------------------------------------- |
| ESP32 Development Board     | Main controller and signal processor    |
| INMP441 I2S MEMS Microphone | Captures acoustic pipe vibrations       |
| 0.96" SSD1306 OLED          | Displays system status and signal level |
| Active Buzzer               | Provides audible alerts                 |
| Breadboard                  | Prototype assembly                      |
| Jumper Wires                | Component connections                   |
| Pipe Clamp                  | Holds microphone firmly against pipe    |
| USB Cable                   | ESP32 programming and power             |

The project concept uses a pipe clamp to improve acoustic coupling between the sensor and pipe surface.

---

## 💻 Software Requirements

* Arduino IDE
* ESP32 Board Package
* C++
* Adafruit GFX Library
* Adafruit SSD1306 Library

### Optional AI Development Tools

For the planned Edge-AI implementation:

* Edge Impulse
* Edge Impulse Data Forwarder / CLI
* MFCC feature extraction
* TinyML model
* Arduino / PlatformIO

---

## 🔌 Pin Configuration

### I2S MEMS Microphone

The current firmware uses the following ESP32 pins:

| INMP441    |   ESP32 |
| ---------- | ------: |
| SCK / BCLK | GPIO 26 |
| WS / LRCL  | GPIO 25 |
| SD         | GPIO 33 |
| VDD        |    3.3V |
| GND        |     GND |

### OLED Display

| OLED |   ESP32 |
| ---- | ------: |
| SDA  | GPIO 21 |
| SCL  | GPIO 22 |
| VCC  |    3.3V |
| GND  |     GND |

### Buzzer

| Buzzer |   ESP32 |
| ------ | ------: |
| Signal | GPIO 27 |
| GND    |     GND |

> Pin assignments can be changed in `acoustic_pipe_leak_detector.ino` if your hardware wiring is different.

---

## 📁 Repository Structure

```text
Acoustic-Edge-AI-Pipe-Leak-Detector/
│
├── acoustic_pipe_leak_detector.ino
├── README.md
└── LICENSE
```

A future TinyML version can be organized as:

```text
Acoustic-Edge-AI-Pipe-Leak-Detector/
│
├── acoustic_pipe_leak_detector.ino
├── README.md
├── LICENSE
│
├── dataset/
│   ├── normal/
│   ├── leak/
│   └── micro_crack/
│
├── model/
│   └── edge_impulse_model/
│
└── docs/
    └── architecture.png
```

---

# ⚙️ How It Works

## 1. Acoustic Signal Capture

The I2S MEMS microphone is mounted against the pipe using a clamp.

The microphone captures acoustic vibrations produced by:

* Normal water flow
* Leakage
* Structural defects
* Other abnormal pipe sounds

The digital audio stream is transferred to the ESP32 through the I2S interface.

---

## 2. Signal Processing

The ESP32 collects a block of audio samples and calculates the signal's RMS level.

Conceptually:

```text
Audio Samples
      ↓
I2S Interface
      ↓
Sample Buffer
      ↓
RMS Calculation
      ↓
Acoustic Signal Level
```

The RMS value provides an estimate of the strength of the captured acoustic signal.

---

## 3. Detection

The prototype uses configurable thresholds to classify abnormal acoustic activity.

```text
             Acoustic Level
                    │
        ┌───────────┴───────────┐
        │                       │
   Low/Normal              High Signal
        │                       │
     NORMAL              ┌──────┴──────┐
                         │             │
                    Medium Level    High Level
                         │             │
                    MICRO-CRACK      LEAK
```

The current firmware contains:

```cpp
#define LEAK_THRESHOLD 450000
#define CRACK_THRESHOLD 250000
```

These values are **prototype thresholds** and should be calibrated using actual sensor measurements.

---

## 4. False Alert Reduction

The system does not immediately trigger an alert from a single abnormal measurement.

Instead, abnormal readings must occur for multiple consecutive measurements:

```cpp
#define DETECTION_COUNT 3
```

For example:

```text
Reading 1 → Abnormal
Reading 2 → Abnormal
Reading 3 → Abnormal
              ↓
          ALERT
```

This helps reduce alerts caused by short-duration acoustic disturbances.

---

## 5. OLED Feedback

The OLED displays:

* System status
* Acoustic signal level
* Detection confidence

Example:

```text
PIPE MONITOR
----------------
Status: NORMAL

Signal: 18432

Confidence: 90%
```

During abnormal activity:

```text
PIPE MONITOR
----------------
Status: LEAK DETECTED

Signal: 521384

Confidence: 95%
```

---

## 6. Buzzer Alert

When a leak or micro-crack condition is detected, the buzzer is activated.

```text
Abnormal Signal
       ↓
Detection Confirmed
       ↓
OLED Alert
       +
Buzzer Alert
```

This allows the system to notify the user without requiring continuous observation of the display.

---

# 🚀 Getting Started

## Step 1 — Install Arduino IDE

Install the Arduino IDE and configure it for ESP32 development.

Select an ESP32-compatible board from:

```text
Tools → Board
```

---

## Step 2 — Install Required Libraries

From:

```text
Sketch → Include Library → Manage Libraries
```

Install:

```text
Adafruit GFX Library
Adafruit SSD1306
```

The ESP32 I2S functionality is provided by the ESP32 board package.

---

## Step 3 — Connect the Hardware

Connect the microphone, OLED and buzzer according to the pin configuration described above.

Make sure the microphone and ESP32 share a common ground.

---

## Step 4 — Open the Firmware

Open:

```text
acoustic_pipe_leak_detector.ino
```

---

## Step 5 — Select the ESP32 Board

For example:

```text
Tools
 → Board
 → ESP32 Arduino
 → ESP32 Dev Module
```

Select the appropriate COM port.

---

## Step 6 — Upload

Compile and upload the firmware to the ESP32.

After successful upload, open:

```text
Tools → Serial Monitor
```

Set the baud rate to:

```text
115200
```

You should see:

```text
System initialized
Acoustic Level: ...
Acoustic Level: ...
Acoustic Level: ...
```

---

# 🧪 Calibration

The detection thresholds depend heavily on:

* Microphone model
* Pipe material
* Pipe diameter
* Water pressure
* Sensor mounting
* Environmental noise
* Distance from the leak
* ESP32 microphone configuration

Therefore, the default thresholds should not be considered universal.

## Collect Normal Readings

Run the system while the pipe is operating normally.

Record:

```text
Acoustic Level
```

from the Serial Monitor.

Example:

```text
Acoustic Level: 12000
Acoustic Level: 14500
Acoustic Level: 16000
Acoustic Level: 18000
```

---

## Collect Leak Readings

Introduce a controlled leak and record the acoustic levels.

Example:

```text
Acoustic Level: 280000
Acoustic Level: 320000
Acoustic Level: 410000
Acoustic Level: 490000
```

The threshold values can then be adjusted based on the observed signal distributions.

---

# 🤖 TinyML / Edge-AI Extension

The project's intended architecture can be extended from threshold-based detection to a trained acoustic classifier.

The proposed workflow is:

```text
                DATA COLLECTION
                       │
                       ▼
              Acoustic Samples
                       │
          ┌────────────┼────────────┐
          ▼            ▼            ▼
        Normal        Leak       Micro-Crack
          │            │            │
          └────────────┼────────────┘
                       ▼
                Feature Extraction
                       │
                      MFCC
                       │
                       ▼
                 Model Training
                       │
                       ▼
                TinyML Classifier
                       │
                       ▼
                  ESP32 Model
                       │
                       ▼
              Real-Time Inference
                       │
             ┌─────────┴─────────┐
             ▼                   ▼
           OLED                Buzzer
```

The project documentation specifies **Edge Impulse** for acoustic dataset preparation, MFCC feature extraction, model training, and deployment to the ESP32.

---

# 📊 Potential Classification Classes

A TinyML dataset can be structured around classes such as:

```text
normal
leak
micro_crack
```

Each class should contain multiple acoustic recordings collected under different operating conditions.

For example:

```text
dataset/
│
├── normal/
│   ├── normal_001.wav
│   ├── normal_002.wav
│   └── ...
│
├── leak/
│   ├── leak_001.wav
│   ├── leak_002.wav
│   └── ...
│
└── micro_crack/
    ├── crack_001.wav
    ├── crack_002.wav
    └── ...
```

A trained classifier can then replace the threshold-based detection logic.

---

# 🧠 Why Edge AI?

Running the classifier directly on the ESP32 provides several potential advantages:

### Local Processing

Audio does not need to be continuously sent to a cloud service for inference.

### Lower Connectivity Dependency

Detection can continue without constant internet connectivity.

### Fast Response

Local inference can provide immediate feedback.

### Compact Architecture

The processing and detection system can remain within the embedded device.

### Privacy

Raw acoustic data can potentially remain on the device rather than being continuously uploaded.

---

# 📈 Current Prototype vs Future Version

| Feature                          | Current Prototype | Future TinyML Version   |
| -------------------------------- | ----------------- | ----------------------- |
| ESP32                            | ✅                 | ✅                       |
| I2S MEMS microphone              | ✅                 | ✅                       |
| OLED                             | ✅                 | ✅                       |
| Buzzer                           | ✅                 | ✅                       |
| Real-time monitoring             | ✅                 | ✅                       |
| RMS signal analysis              | ✅                 | Optional                |
| Threshold detection              | ✅                 | Optional                |
| MFCC extraction                  | Planned           | ✅                       |
| TinyML classifier                | Planned           | ✅                       |
| Edge Impulse                     | Planned           | ✅                       |
| Normal/Leak/Crack classification | Threshold-based   | ML-based                |
| Model confidence                 | Prototype value   | Actual model confidence |


# ⚠️ Limitations

This repository contains a **prototype implementation**.

The threshold-based firmware does not constitute a trained AI model.

Detection performance can vary depending on:

* Pipe construction
* Sensor placement
* Background noise
* Water flow
* Leak size
* Crack characteristics
* Sensor sensitivity
* Threshold calibration

The values:
LEAK_THRESHOLD
CRACK_THRESHOLD

must be experimentally calibrated for the specific hardware and environment.

The system should therefore be treated as a **prototype/research project**, not as a certified industrial pipeline monitoring or safety system.

# 🔧 Future Improvements
Potential improvements include:
* Train a dedicated TinyML acoustic classifier.
* Add MFCC-based feature extraction.
* Integrate the exported Edge Impulse model.
* Add a larger and more diverse acoustic dataset.
* Implement noise filtering.
* Add temperature and pressure sensors.
* Add Wi-Fi-based monitoring.
* Add MQTT communication.
* Store detection events.
* Add battery operation.
* Design a custom PCB.
* Improve the pipe clamp mechanism.
* Test different pipe materials and diameters.
* Evaluate detection accuracy using controlled experiments.
* Add confidence-based alert thresholds.

# 📷 Project Architecture
The overall architecture can be summarized as:

             ┌─────────────────┐
             │      PIPE       │
             │                 │
             │ Flow / Leak /   │
             │    Defect       │
             └────────┬────────┘
                      │
                      ▼
             ┌─────────────────┐
             │   I2S MEMS      │
             │   MICROPHONE    │
             └────────┬────────┘
                      │
                  I2S Audio
                      │
                      ▼
             ┌─────────────────┐
             │      ESP32      │
             │                 │
             │ Signal Process  │
             │       +         │
             │ Detection       │
             └───────┬─────────┘
                     │
              ┌──────┴──────┐
              │             │
              ▼             ▼
       ┌────────────┐  ┌────────────┐
       │    OLED    │  │   BUZZER   │
       │  DISPLAY   │  │   ALERT    │
       └────────────┘  └────────────┘

# 🛠️ Troubleshooting

## OLED is not displaying anything
Check:
SDA → GPIO 21
SCL → GPIO 22
VCC → 3.3V
GND → GND

The firmware uses OLED address:

Some displays may use a different I2C address.

## Microphone readings are always zero

Check:

* I2S wiring
* Microphone power
* Common ground
* BCLK connection
* WS/LRCL connection
* Data connection
* Correct microphone channel

The current firmware assumes an **INMP441-style I2S microphone configuration**.

## Micro-crack detection is unreliable
Micro-cracks can produce subtle acoustic signatures. A simple amplitude threshold may not reliably distinguish them from normal background noise.
For improved classification, use a trained acoustic model with:
Audio
 ↓
MFCC
 ↓
TinyML Model
 ↓
Normal / Leak / Micro-Crack

# 📚 Project Technologies
ESP32
Arduino C++
I2S
MEMS Microphone
OLED
Embedded Systems
Signal Processing
TinyML
Edge AI
Edge Impulse
MFCC
IoT

# ⭐ Acknowledgements

This project was developed as an academic/capstone IoT prototype exploring acoustic sensing, embedded signal processing, and Edge-AI-based fault detection.
The proposed system architecture uses an **I2S MEMS sensor, ESP32, TinyML acoustic classification, OLED display, and buzzer alerts** for real-time monitoring.
