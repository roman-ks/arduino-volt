// include TFT and SPI libraries
#include <TFT.h>  
#include <SPI.h>
#include    <stdlib.h>

// pin definition for Arduino UNO
#define cs   53
#define dc   22
#define rst  8

int analogInput = 0;
int inputs[] = {87,88,89,90,91,92,93,94,95,96};
int rs[] = {1,2,3,4,5};
char strs[10][20];


// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);
int val_size = 10;
float vals[10];

void setup() {
//  pinMode(14, INPUT);
  for(int i=0;i<val_size;i++){
     pinMode(inputs[i], INPUT);
  }
 
  //initialize the library
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  //set the text size
  TFTscreen.setTextSize(2);
}


void loop() {
//  TFTscreen.background(0, 0, 0);
  // first col
  for(int i=0;i<5;i++){
    int value = analogRead(inputs[i]);
    float vout = (value * 5.0) / 1024.0 * rs[i];

    for(int j=0; j<i-1; j++){
      vout = vout - vals[j];
    }
    vals[i]=vout;
    
    dtostrf(vout, 2, 2, strs[i]);
  }

  // second col
  for(int i=5;i<10;i++){
    int value = analogRead(inputs[i]);
    float vout = (value * 5.0) / 1024.0 * rs[i-5];

    for(int j=5; j<i-1; j++){
      vout = vout - vals[j];
    }
    vals[i]=vout;
    
    dtostrf(vout, 2, 2, strs[i]);
  }


  for(int i=0;i<10;i++){
    int col = i/5;
    
    if(vals[i]<=3.85){
      TFTscreen.stroke(10, 10, 255);
    }else if(vals[i]>3.85 && vals[i]<=4.0){
      TFTscreen.stroke(10, 255, 255);
    }else {
      TFTscreen.stroke(10, 255, 10);
    }

    // clear rect
    TFTscreen.fill(0,0,0);
    TFTscreen.rect(col*80, 25*(i-5*col), 80, 25);
    TFTscreen.text(strs[i], 6+col*80, 25*(i-5*col)+5);
    delay(50);
  }
  
  delay(500);
  
}
