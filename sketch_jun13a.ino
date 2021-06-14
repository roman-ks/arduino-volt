// include TFT and SPI libraries
#include <TFT.h>  
#include <SPI.h>
#include    <stdlib.h>

// pin definition for Arduino UNO
#define cs   10
#define dc   9
#define rst  8

int analogInput = 0;
int inputs[] = {14,15,16,17,18,14,15,16,17,18};
int rs[] = {1,1,1,1,1,1,1,1,1,1};
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
  TFTscreen.background(0, 0, 0);
  
  for(int i=0;i<10;i++){
    int value = analogRead(inputs[i]);
    float vout = (value * 5.0) / 1024.0; // see text
//    float vin = vout / rs[i];
//    vals[i]=vin; 
    vals[i]=vout;
    dtostrf(vout, 2, 2, strs[i]);
  }


  for(int i=0;i<10;i++){
    if(vals[i]<=3.85){
      TFTscreen.stroke(10, 10, 255);
    }else if(vals[i]>3.85 && vals[i]<=4.0){
      TFTscreen.stroke(10, 255, 255);
    }else {
      TFTscreen.stroke(10, 255, 10);
    }

    int col = i/5;
    TFTscreen.text(strs[i], 6+col*80, 25*(i-5*col)+5);
  }
  
  delay(100);
  
}
