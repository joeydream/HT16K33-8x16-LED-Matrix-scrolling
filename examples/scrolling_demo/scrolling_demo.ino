#include <matrix_LED.h>
#define Addr 0x70

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(Addr);   // 1. 啟動 VK16K33 晶片振盪器
  Wire.write(0x21);
  Wire.endTransmission();
  Wire.beginTransmission(Addr);   // 2. 開啟顯示 (無閃爍)
  Wire.write(0x81);
  Wire.endTransmission();
}

void loop() {
  uint8_t phraseBuffer[220]={0};
  uint8_t nowVertScreen[16] = {0};
  setBrightness(8);
  int totalCols = aprphrase("Today is Monday.", phraseBuffer);
  for (int startByte = 0; startByte < totalCols + 16; startByte++){
    get16byte(phraseBuffer, nowVertScreen, startByte, totalCols);
    instantdraw(nowVertScreen);
    delay(150);
  }
  clearDisplay();
  delay(3000);
}