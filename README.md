# HT16K33 8x16 LED Matrix Scrolling Library for Arduino

A lightweight, bare-metal scrolling text library designed for the HT16K33 16x8 LED matrix module, optimized for Arduino.

## Features
* **Bare-metal implementation**: No heavy third-party graphics dependencies.
* **Smooth scrolling**: Efficient byte-level manipulation for smooth text marquee effects.
* **Custom Font Included**: Built-in 57-ASCII character set (16x8 layout support).

---

## Hardware Connections (Wiring)

Connect your HT16K33 16x8 LED matrix to your Arduino (e.g., Arduino Nano / Uno) via I2C:

| HT16K33 Pin | Arduino Nano / Uno Pin |
| :--- | :--- |
| **VCC** | 5V |
| **GND** | GND |
| **SDA** | A4 |
| **SCL** | A5 |

*Note: Default I2C address is `0x70`.*

---

## Installation

1. Click the green **`<> Code`** button on this repository and select **Download ZIP**.
2. Open your Arduino IDE.
3. Go to **Sketch > Include Library > Add .ZIP Library...** and select the downloaded ZIP file.

---

## Quick Start Example

Here is a simple example to get your scrolling text running:

```cpp
#include <OttoMatrixLED.h>
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // 1. Initialize HT16K33 oscillator
  Wire.beginTransmission(Addr);
  Wire.write(0x21); 
  Wire.endTransmission();
  
  // 2. Turn on display (no blinking)
  Wire.beginTransmission(Addr);
  Wire.write(0x81); 
  Wire.endTransmission();
}

void loop() {
  uint8_t phraseBuffer[220] = {0};
  uint8_t nowVertScreen[16] = {0};
  
  setBrightness(8); // Set brightness (0 - 15)
  
  int totalCols = aprphrase("HT16K33 Ready!", phraseBuffer);
  
  for (int startByte = 0; startByte < totalCols + 16; startByte++) {
    get16byte(phraseBuffer, nowVertScreen, startByte, totalCols);
    instantdraw(nowVertScreen);
    delay(150);
  }
  
  clearDisplay();
  delay(1000);
}
