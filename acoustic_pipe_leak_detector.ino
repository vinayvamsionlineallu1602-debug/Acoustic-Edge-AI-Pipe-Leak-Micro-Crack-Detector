```cpp
/*
 * Acoustic Edge-AI Pipe Leak & Micro-Crack Detector
 * -------------------------------------------------
 * Hardware:
 *   - ESP32 Development Board
 *   - I2S MEMS Microphone (INMP441)
 *   - 0.96" OLED SSD1306 I2C Display
 *   - Active Buzzer
 *
 * File: acoustic_pipe_leak_detector.ino
 *
 * Description:
 *   Captures acoustic signals from an I2S MEMS microphone,
 *   calculates signal energy, detects abnormal acoustic
 *   activity, and provides visual/audible alerts.
 *
 * Note:
 *   This version is a standalone prototype and does not
 *   require an external TinyML model.
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "driver/i2s.h"

// -----------------------------
// OLED Configuration
// -----------------------------

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

// -----------------------------
// I2S Microphone Configuration
// -----------------------------

#define I2S_PORT I2S_NUM_0

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 26

#define SAMPLE_RATE 16000
#define SAMPLE_COUNT 1024

int32_t samples[SAMPLE_COUNT];

// -----------------------------
// Buzzer
// -----------------------------

#define BUZZER_PIN 27

// -----------------------------
// Detection Configuration
// -----------------------------

// Adjust these values after collecting
// normal and leak-condition samples.

#define LEAK_THRESHOLD 450000
#define CRACK_THRESHOLD 250000

// Number of consecutive abnormal
// readings required before alerting.

#define DETECTION_COUNT 3

int leakCounter = 0;
int crackCounter = 0;

// -----------------------------
// I2S Setup
// -----------------------------

void setupI2S()
{
    i2s_config_t i2s_config;

    i2s_config.mode =
        (i2s_mode_t)(
            I2S_MODE_MASTER |
            I2S_MODE_RX
        );

    i2s_config.sample_rate = SAMPLE_RATE;

    i2s_config.bits_per_sample =
        I2S_BITS_PER_SAMPLE_32BIT;

    i2s_config.channel_format =
        I2S_CHANNEL_FMT_ONLY_LEFT;

    i2s_config.communication_format =
        I2S_COMM_FORMAT_I2S;

    i2s_config.intr_alloc_flags = 0;

    i2s_config.dma_buf_count = 8;
    i2s_config.dma_buf_len = 256;

    i2s_config.use_apll = false;
    i2s_config.tx_desc_auto_clear = false;
    i2s_config.fixed_mclk = 0;

    i2s_pin_config_t pin_config;

    pin_config.bck_io_num = I2S_SCK;
    pin_config.ws_io_num = I2S_WS;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = I2S_SD;

    i2s_driver_install(
        I2S_PORT,
        &i2s_config,
        0,
        NULL
    );

    i2s_set_pin(
        I2S_PORT,
        &pin_config
    );

    i2s_zero_dma_buffer(I2S_PORT);
}

// -----------------------------
// Read Acoustic Samples
// -----------------------------

float readAcousticLevel()
{
    size_t bytesRead = 0;

    i2s_read(
        I2S_PORT,
        samples,
        sizeof(samples),
        &bytesRead,
        portMAX_DELAY
    );

    int sampleCount =
        bytesRead / sizeof(int32_t);

    if (sampleCount == 0)
        return 0;

    double energy = 0;

    for (int i = 0; i < sampleCount; i++)
    {
        // Convert 32-bit I2S sample
        // to a usable signed value.
        int32_t sample =
            samples[i] >> 8;

        energy +=
            (double)sample *
            (double)sample;
    }

    double rms =
        sqrt(energy / sampleCount);

    return (float)rms;
}

// -----------------------------
// OLED Display
// -----------------------------

void showDisplay(
    const char* status,
    float level,
    int confidence
)
{
    display.clearDisplay();

    display.setTextColor(
        SSD1306_WHITE
    );

    display.setTextSize(1);

    display.setCursor(0, 0);
    display.println(
        "PIPE MONITOR"
    );

    display.drawLine(
        0, 10,
        127, 10,
        SSD1306_WHITE
    );

    display.setCursor(0, 18);
    display.print("Status: ");
    display.println(status);

    display.setCursor(0, 32);
    display.print("Signal: ");
    display.println(level, 0);

    display.setCursor(0, 46);
    display.print("Confidence: ");
    display.print(confidence);
    display.println("%");

    display.display();
}

// -----------------------------
// Buzzer Alert
// -----------------------------

void alertBuzzer()
{
    digitalWrite(
        BUZZER_PIN,
        HIGH
    );

    delay(250);

    digitalWrite(
        BUZZER_PIN,
        LOW
    );
}

// -----------------------------
// Setup
// -----------------------------

void setup()
{
    Serial.begin(115200);

    pinMode(
        BUZZER_PIN,
        OUTPUT
    );

    digitalWrite(
        BUZZER_PIN,
        LOW
    );

    // OLED initialization
    Wire.begin(
        OLED_SDA,
        OLED_SCL
    );

    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            0x3C))
    {
        Serial.println(
            "OLED initialization failed"
        );

        while (true)
        {
            delay(1000);
        }
    }

    display.clearDisplay();

    display.setTextColor(
        SSD1306_WHITE
    );

    display.setTextSize(1);

    display.setCursor(0, 20);

    display.println(
        "Acoustic Pipe"
    );

    display.setCursor(0, 35);

    display.println(
        "Leak Detector"
    );

    display.display();

    delay(2000);

    // Initialize microphone
    setupI2S();

    Serial.println(
        "System initialized"
    );
}

// -----------------------------
// Main Loop
// -----------------------------

void loop()
{
    float acousticLevel =
        readAcousticLevel();

    Serial.print(
        "Acoustic Level: "
    );

    Serial.println(
        acousticLevel
    );

    // ---------------------------------
    // Leak Detection
    // ---------------------------------

    if (acousticLevel >
        LEAK_THRESHOLD)
    {
        leakCounter++;
    }
    else
    {
        leakCounter = 0;
    }

    // ---------------------------------
    // Micro-Crack Detection
    // ---------------------------------

    if (
        acousticLevel >
        CRACK_THRESHOLD &&
        acousticLevel <=
        LEAK_THRESHOLD
    )
    {
        crackCounter++;
    }
    else
    {
        crackCounter = 0;
    }

    // ---------------------------------
    // Determine Condition
    // ---------------------------------

    if (leakCounter >=
        DETECTION_COUNT)
    {
        Serial.println(
            "ALERT: PIPE LEAK DETECTED"
        );

        showDisplay(
            "LEAK DETECTED",
            acousticLevel,
            95
        );

        alertBuzzer();

        leakCounter = 0;
    }

    else if (crackCounter >=
             DETECTION_COUNT)
    {
        Serial.println(
            "WARNING: MICRO-CRACK"
        );

        showDisplay(
            "MICRO-CRACK",
            acousticLevel,
            80
        );

        alertBuzzer();

        crackCounter = 0;
    }

    else
    {
        showDisplay(
            "NORMAL",
            acousticLevel,
            90
        );
    }

    delay(100);
}
