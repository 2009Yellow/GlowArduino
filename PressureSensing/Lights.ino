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
int numContactAreas = 4;
int columns[] = {52, 104, 156, 208, 260, 312, 364, 416, 468, 520, 572}; //max pixel num for each column, use for hexagon calculation

void initLights() {
  // Initialize the light pins
  // Initialize the light state
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
  for (int i = 0; i < 4; ++i) {
    int value = 0;
    // Read high byte
    value += ((int)waitToRead())*256;
    // Read low byte
    value += ((int)waitToRead());
    // Update value
    if(lightLocs[i] != value){
      lightLocs[i] = value;
      pixelsOff();
    }
    //strip.setPixelColor(value, strip.Color(0, 100, 0));
    //strip.show();
  }

  // Read color values
  for (int i = 0; i < 4; ++i) {
    lightColors[i] = ((int)waitToRead());
  }
  
  char finalChar = waitToRead();
  //Serial.println("finalChar: " + String(finalChar)); 
  if (finalChar != SERIAL_LIGHT_FINAL_RECEIVE_CHAR) {
    Serial.write(SERIAL_LIGHT_ERROR_CHAR);
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
