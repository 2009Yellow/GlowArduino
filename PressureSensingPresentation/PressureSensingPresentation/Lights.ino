#include "LPD8806.h"
#include "SPI.h"

// Light Constants
#define STRIP_PURPLE (strip.Color(60, 10, 120))
#define STRIP_RED (strip.Color(100, 10, 10))
#define STRIP_WHITE (strip.Color(50, 50, 50))

//Light Initialization
int dataPin = 6;
int clockPin = 7;
int nLEDs = 572;
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

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
      pixelsOff();
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



void updateLights(){
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

  if (count == 4){
    lightEdges();
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

  fadeGlow();
  //Number gives timespan of effect. Color 1 is wipe color, Color 2 is "GLOW" color
  delay(1000);
}

void All_off(){
  for (int i = 0; i < 573; i++){
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
}


int translate(int ledLoc){
  return ((11-(ledLoc/52))*52 + (ledLoc%52));
}


void fadeGlow(){
  int pixels[] = {
    101, 99, 97, 95, 93, 88, 86, 84, 82, 80, 75, 73, 71, 69, 67, 60, 54, 105, 115, 119, 131, 141, 146, 148, 152, 154, 206, 201, 196, 192, 180, 170, 166, 162, 160, 156, 209, 214, 216, 218, 223, 235, 245, 249, 254, 259, 310, 296, 284, 274, 270, 260, 313, 327, 339, 349, 353, 363, 414, 404, 400, 388, 378, 374, 364, 417, 427, 431, 443, 453, 457, 467, 517, 515, 513, 511, 509, 504, 491, 489, 487, 485, 483, 478, 468  };

  int red = 0;
  int blue = 0;
  int green = 0;
  while(red < 120){
    for(int i = 0; i < 84; i ++){
      strip.setPixelColor(translate(letters[i]), strip.Color(red, green, 0));
    }
    strip.show();
    red = red + 1;
    green = green + 1; 
    delay(30);
  }

}

