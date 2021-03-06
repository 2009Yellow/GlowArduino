#include "LPD8806.h"
#include "SPI.h"

// Light Constants
#define STRIP_PURPLE (strip.Color(60, 10, 120))
#define STRIP_RED (strip.Color(100, 10, 10))
#define STRIP_WHITE (strip.Color(50, 50, 50))
#define STRIP_GREEN (strip.Color(10, 100, 10))

//Light Initialization
int dataPin = 6;
int clockPin = 7;
int nLEDs = 572;
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);
boolean hasDoneAPose = false;

// light data
int lightLocs [4];
int lightColors[4];
int lightDirections[4];
int numContactAreas = 4;
int columns[] = {
  52, 104, 156, 208, 260, 312, 364, 416, 468, 520, 572}; //max pixel num for each column, use for hexagon calculation

void initLights() {
  // Initialize the light pins
  // Initialize the light state
  pixelsOff();
  strip.begin();
  strip.show();
}

char waitToRead() {
  while (Serial.available() == 0) {
  }
  return Serial.read();
}

void receiveLightData() {
  
  char firstChar = waitToRead();
  //Serial.println("firstChar: " + String(firstChar)); 
  if (firstChar != SERIAL_LIGHT_FIRST_RECEIVE_CHAR) {
    Serial.write(SERIAL_LIGHT_ERROR_CHAR);
    return;
  }
  
  // Read position values
  digitalWrite(13,HIGH);
  for (int i = 0; i < 4; ++i) {
    int value = 0;
    // Read high byte
    value += ((int)waitToRead())*128;
    // Read low byte
    value += ((int)waitToRead());
    // Update value
    if(lightLocs[i] != value){
      lightLocs[i] = value;
      //pixelsOff();
      All_off();
      //drawPressureButtons();
    }
    //strip.setPixelColor(value, strip.Color(0, 100, 0));
    //strip.show();
  }
  // Read color values
  for (int i = 0; i < 4; ++i) {
    lightColors[i] = ((int)waitToRead());
  }

  /*for (int i = 0; i<4; ++i) {
   lightDirections[i] = ((int)waitToRead());    
   }*/

  char finalChar = waitToRead();
  //Serial.println("finalChar: " + String(finalChar)); 
  if (finalChar != SERIAL_LIGHT_FINAL_RECEIVE_CHAR) {
    //Serial.write(SERIAL_LIGHT_ERROR_CHAR);
    return;
  }

  /*
  for(int i; i < 4; ++i) {
   Serial.println(String(lightLocs[i]));
   }
   for(int i; i < 4; ++i) {
   Serial.println(String(lightColors[i]));
   }
   */
}



/*void updateLights(){
  int count = 0;
  for (int i = 0; i <4; i ++){
    if(lightLocs[i] !=0){
      switch(lightColors[i]){
      case 1:
        makeHexagon(lightLocs[i], STRIP_PURPLE);
        break;
      case 2:
        makeHexagon(lightLocs[i], STRIP_WHITE);
        count++;
        break;
      case 3:
        makeHexagon(lightLocs[i], STRIP_RED);
        break;
      }
    }
  }

  strip.show();
}*/

//CHANGED THIS FOR DIFFERENT COLORS AND SHAPES
void updateLights(){
  boolean handsOnMat = true;
  for(int i = 0; i<4; i++){
    if(lightLocs[i]== 0){
      handsOnMat = false; //for our limited number of poses
    }
  }
  for (int i = 0; i <4; i ++){
    if(lightLocs[i] != 0){
      switch(lightColors[i]){
      case 1:
        if(handsOnMat && i<2){
         makeDiamond(lightLocs[i], STRIP_WHITE);
         break;
         } 
        makeHexagon(lightLocs[i], STRIP_WHITE);
        break;

      case 2:
        if(handsOnMat && i<2){
         makeDiamond(lightLocs[i], STRIP_WHITE);
         break;
        }
        makeHexagon(lightLocs[i], STRIP_WHITE);
        break;

      case 3:
        if(handsOnMat && i<2){
         makeDiamond(lightLocs[i], STRIP_RED);
         break;
         }
        makeHexagon(lightLocs[i], STRIP_RED);
        break;
        
      case 4:
        if(handsOnMat && i<2){
         makeDiamond(lightLocs[i], STRIP_GREEN);
         break;
         }
        makeHexagon(lightLocs[i], STRIP_GREEN);
        break;
      }
    }
  }
  strip.show();
}

void makeDiamond(int pixel, uint32_t c){
  int row;
  int col;
  int pixels[4];
  pixels[0] = pixel + 2;
  pixels[1] = pixel - 2;

  for (int i = 0; i < 10; i ++){
    if (pixel < columns[i]){
      col = i;
      break;
    }
  }

  if (col % 2 == 0){
    row = pixel - columns[col - 1];
    pixels[2] = columns[col - 1] - row -1;
    pixels[3] = columns[col + 1] - row -1;

  }
  else {
    row = columns[col] - pixel;
    pixels[2] = columns[col - 2] + row - 1;
    pixels[3] = columns[col] + row - 1;
  }

  for(int i = 0; i < 6; i ++){
    strip.setPixelColor(pixels[i], c);
  }
  strip.show();
}

void lightEdges(){
  for(int i = 0; i < 52; i ++){
    strip.setPixelColor(i, STRIP_WHITE);
    strip.setPixelColor(i + 520, STRIP_WHITE);
  }
}

void pixelsOff(){
  for(int i = 0; i < strip.numPixels(); i ++){
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

void makeHexagon(int pixel, uint32_t c){
  int row;
  int col;
  int pixels[6];
  pixels[0] = pixel + 2;
  pixels[1] = pixel - 2;

  for (int i = 0; i < 10; i ++){
    if (pixel < columns[i]){
      col = i;
      break;
    }
  }

  if (col % 2 == 0){
    row = pixel - columns[col - 1];
    pixels[2] = columns[col - 1] - row - 2;
    pixels[3] = columns[col - 1] - row ;
    pixels[4] = columns[col + 1] - row ;
    pixels[5] = columns[col + 1] - row - 2;

  }
  else {
    row = columns[col] - pixel;
    pixels[2] = columns[col - 2] + row - 2;
    pixels[3] = columns[col - 2] + row ;
    pixels[4] = columns[col] + row ;
    pixels[5] = columns[col] + row - 2;
  }

  for(int i = 0; i < 6; i ++){
    strip.setPixelColor(pixels[i], c);
  }
  strip.show();
}

void drawPressureButtons(){

  int rightPixels[] = {
    2, 101, 106, 107, 108, 109, 98, 5, 4, 3  };
  int leftPixels[] = {
    12, 91, 116, 117, 118, 119, 88, 15, 14, 13  };
  for (int i = 0; i < sizeof(rightPixels); i++){
    strip.setPixelColor(rightPixels[i], STRIP_WHITE);
    strip.setPixelColor(leftPixels[i], STRIP_WHITE);
  }
  strip.show();
}

/*

 void receiveLightData() {
 
 char input = Serial.read();
 if (input != SERIAL_LIGHT_FIRST_RECEIVE_CHAR) {
 char buffer[10];
 int trash = Serial.readBytesUntil((char)SERIAL_LIGHT_FINAL_RECEIVE_CHAR, buffer, 100);
 return;
 }
 
 //reading in light locations
 for( int i = 0; i<sizeof(lightLocs); i++ ){
 int integerValue = 0;
 while(1) {            // force into a loop until 'n' is received
 byte high_byte = Serial.read();
 if (high_byte == -1) {i--; continue;}  // if no characters are in the buffer read() returns -1
 byte low_byte = Serial.read();
 if (low_byte == -1) {low_byte = 0;}
 integerValue = (low_byte + (high_byte << 8));
 }
 lightLocs[i] = (integerValue);   // Do something with the value
 }
 
 char nextVal = Serial.read();
 if(nextVal != SERIAL_LIGHT_SWITCH_TO_COLOR_CHAR) {
 return;
 }
 
 Serial.readBytesUntil(SERIAL_LIGHT_FINAL_RECEIVE_CHAR, lightColors, 4);
 }
 

 
 
 */

void startSequence() {
  All_off();
  lightLocs[0] = 572; //so that the next pose will definitely turn off LED's
  fadeGlow();
  //Number gives timespan of effect. Color 1 is wipe color, Color 2 is "GLOW" color
  delay(100);
}

void All_off(){
  for (int i = 0; i < 573; i++){
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();
}


int translate(int ledLoc){
  return ((11-(ledLoc/52))*52 + 51-(ledLoc%52));
}


void fadeGlow(){
  int pixels[] = {106, 108, 110, 112, 114, 197, 219, 301, 303, 305, 214, 206, 209, 310, 313, 414, 417, 518, 522, 524, 526, 528, 530, 508, 427, 119, 192, 223, 296, 327, 400, 431, 504, 535, 121, 123, 125, 127, 132, 134, 136, 138, 140, 170, 245, 274, 349, 378, 453, 482, 556, 554, 552, 550, 548, 492, 443, 388, 339, 284, 235, 180, 561, 478, 457, 374, 353, 270, 249, 165, 147, 163, 253, 265, 255, 159, 153, 157, 259, 260, 363, 364, 467, 468, 571};

  int red = 0;
  int blue = 0;
  int green = 0;
  while(red < 120){
    for(int i = 0; i < 84; i ++){
      strip.setPixelColor(translate(pixels[i]), strip.Color(red, green, 0));
    }
    strip.show();
    red = red + 1;
    green = green + 1; 
    delay(30);
  }

}

