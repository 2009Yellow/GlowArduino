#include "LPD8806.h"
#include "SPI.h"

// Light Constants
#define STRIP_PURPLE (strip.Color(30, 0, 60))
#define STRIP_RED (strip.Color(120, 30, 30))
#define STRIP_WHITE (strip.Color(30, 30, 30))

//Light Initialization
int dataPin = 2;
int clockPin = 3;
int nLEDs = 572;
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// light data
int lightLocs [4];
int lightColors[4];
int numContactAreas = 4;

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

void recieveLightData() {

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
    value += ((int)waitToRead()) << 8;
    // Read low byte
    value += ((int)waitToRead());
    // Update value
    lightLocs[i] = value;
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
  for (int i = 0; i <4; i ++){
    if(lightColors[i] !=0){
    switch(lightColors[i]){
      case 1:
        strip.setPixelColor(lightLocs[i], strip.Color(60, 10, 120));
        break;
      case 2:
        strip.setPixelColor(lightLocs[i], strip.Color(50, 50, 50));
        break;
      case 3:
        strip.setPixelColor(lightLocs[i], strip.Color(100, 10, 10));
        break;
    }
    }
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
