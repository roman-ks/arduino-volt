// include TFT and SPI libraries
#include <SPI.h>
#include <TFT.h>
#include <stdlib.h>

// pin definition for Arduino MEGA
#define cs 53
#define dc 22
#define rst 8

#define EPSILON 0.01
#define CL_HEIGHT 20
#define CL_WIDTH 80
#define COLUMNS 2
#define ROWS 5  // rows without sum row
#define VALUE_CELLS ROWS* COLUMNS

int inputs[] = {87, 88, 89, 90, 97, 92, 93, 94, 98, 96};
int rs[] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
bool refresh[VALUE_CELLS];
bool sumRefresh[COLUMNS];
char strs[VALUE_CELLS][VALUE_CELLS];
char sumStrs[COLUMNS][VALUE_CELLS];

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);
float vals[VALUE_CELLS];
float sumVals[COLUMNS];

void setup() {
  Serial.begin(9600);
  Serial.print("setup");
  Serial.println();

  for (int i = 0; i < VALUE_CELLS; i++) {
    pinMode(inputs[i], INPUT);
  }

  // initialize the library
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  // set the text size
  TFTscreen.setTextSize(2);
}

void loop() {
  Serial.print("loop");
  Serial.println();
  // first and second col
  for (int i = 0; i < VALUE_CELLS; i++) {
    int value = analogRead(inputs[i]);
    float vout = (value * 5.0) / 1024.0 * rs[i];

    Serial.print("Got value :");
    Serial.print(vout);
    Serial.print("On pin ");
    Serial.print(inputs[i]);
    Serial.println();

    // sum will be in the last cell, save into separate array
    if (i % ROWS == ROWS - 1) {
      int offset = i / ROWS;
      setNewValue(vout, sumVals + offset, sumRefresh + offset, sumStrs[offset]);
    }

    int colStart = i / ROWS * ROWS;
    for (int j = colStart; j < i; j++) {
      Serial.print(" -");
      Serial.print(vals[j]);
      vout = vout - vals[j];
    }
    Serial.println();
    setNewValue(vout, vals + i, refresh + i, strs[i]);
  }

  Serial.print("First half: ");
  printArr(vals, ROWS);
  Serial.println();

  Serial.print("Second half: ");
  printArr(vals + ROWS, ROWS);
  Serial.println();

  for (int i = 0; i < VALUE_CELLS; i++) {
    if (!refresh[i]) continue;
    int col = i / ROWS;

    if (vals[i] <= 3.85) {
      TFTscreen.stroke(10, 10, 255);
    } else if (vals[i] > 3.85 && vals[i] <= 4.0) {
      TFTscreen.stroke(10, 255, 255);
    } else {
      TFTscreen.stroke(10, 255, 10);
    }

    // clear rect
    TFTscreen.fill(0, 0, 0);
    TFTscreen.rect(col * CL_WIDTH, CL_HEIGHT * (i - 5 * col), CL_WIDTH,
                   CL_HEIGHT);
    TFTscreen.text(strs[i], 6 + col * CL_WIDTH, CL_HEIGHT * (i - 5 * col) + 5);
  }

  for (int i = 0; i < COLUMNS; i++) {
    if (!sumRefresh[i]) continue;
    TFTscreen.stroke(255, 255, 255);
    TFTscreen.rect(i * CL_WIDTH, CL_HEIGHT * 5, CL_WIDTH, CL_HEIGHT);
    TFTscreen.text(sumStrs[i], 6 + i * CL_WIDTH, CL_HEIGHT * 5 + 5);
  }

  delay(500);
}

void setNewValue(float val, float* valPtr, bool* refreshPtr, char* strVal) {
  bool isNew = fabs(*valPtr - val) >= EPSILON;
  if (isNew) {
    *valPtr = val;
    dtostrf(val, 2, 2, strVal);
  }
  *refreshPtr = isNew;
}

void printArr(float* arr, int size) {
  Serial.print("[");
  for (int i = 0; i < size - 1; i++) {
    Serial.print(arr[i]);
    Serial.print(", ");
  }
  Serial.print(arr[size - 1]);
  Serial.print("]");
}
