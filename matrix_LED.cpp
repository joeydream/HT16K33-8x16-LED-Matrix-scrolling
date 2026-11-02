#include "HT16K33-8x16-LED-Matrix-scrolling"

void setBrightness(uint8_t level) {
  if (level > 15) level = 15;
  Wire.beginTransmission(Addr);
  Wire.write(0xE0 | level); // VK16K33 亮度指令 0xE0 ~ 0xEF
  Wire.endTransmission();
}

void clearDisplay() {
  Wire.beginTransmission(Addr);
  Wire.write(0x00);
  for (int i = 0; i < 16; i++) {
    Wire.write(0x00);
  }
  Wire.endTransmission();
}

int aprphrase(const char *text, uint8_t *output) {
  int outIndex = 0;
  for (int i = 0; i <16; i++){
    output[outIndex++] = 0x00;
  }
    bool isFirstWordLetter = true; // 用來追蹤是不是單詞的第一個字母，決定空格數
  for (int k = 0; text[k] != '\0'; k++) {
    char c = text[k];

    if (c == ' ') {
      for (int s = 0; s < 3; s++) {
        output[outIndex++] = 0x00;
      }
      isFirstWordLetter = true; // 下一個字母視為新單詞的開頭
    } 
    else {
      if (!isFirstWordLetter) {
        output[outIndex++] = 0x00;
      }
      int fontIndex = c - 32;
      if(fontIndex < 0 || fontIndex >= 95){
        fontIndex = 0;
      }
      for (int i = 0; i < 5; i++){
        output[outIndex++] = pgm_read_byte(&(font5x7_table[fontIndex][i]));
      }
      isFirstWordLetter = false; // 已經不是第一個字母了
    }
  }
  return outIndex; // 返回總行數 n
}


void get16byte(uint8_t *vS, uint8_t *nVS, int startByte, int totalCols) {
  for (int i = 0; i < 16; i++) nVS[i] = 0;
  for (int i = 0; i < 16; i++) {
    int targetIndex = startByte + i;
    if(targetIndex < totalCols){
      nVS[i] = vS[targetIndex];
    }else{
      nVS[i] = 0x00;
    } 
  }
}

void transposeBits(const uint8_t *input, uint8_t *output) {
  for (int i = 0; i < 8; i++) {
    uint8_t new_val = 0;
    for (int j = 0; j < 8; j++) {
      uint8_t bit = (input[j] >> (7 - i)) & 0x01;
      new_val |= (bit << j);
    }
    output[i] = new_val;
  }
  for (int i = 0; i < 8; i++) {
    uint8_t new_val = 0;
    for (int j = 0; j < 8; j++) {
      uint8_t bit = (input[8 + j] >> (7 - i)) & 0x01;
      new_val |= (bit << j);
    }
    output[8 + i] = new_val;
  }
}

void instantdraw(const uint8_t *nVS) {
  uint8_t transposedData[16] = {0};
  uint8_t rearrangedData[16] = {0};
  transposeBits(nVS, transposedData);
  for(int i = 0; i < 8; i++){
    rearrangedData[2*i] = transposedData[i];
    rearrangedData[2*i+1] = transposedData[8+i];
  }
  Wire.beginTransmission(Addr);
  Wire.write(0x00);
  for (int i = 0; i < 16; i++) {
    Wire.write(rearrangedData[i]);
  }
  Wire.endTransmission();
}
#endif