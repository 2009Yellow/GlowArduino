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

// light data
int lightLocs [4];
int lightColors[4];
int lightDirections[4];
int numContactAreas = 4;
int columns[] = {52, 104, 156, 208, 260, 312, 364, 416, 468, 520, 572}; //max pixel num for each column, use for hexagon calculation

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
  
  /*
  for (int i = 0; i<4; ++i) {
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
  
  if (count == 4){
    lightEdges();
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

  for(int i = 0; i < 4; i ++){
    strip.setPixelColor(pixels[i], c);
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
  
  int rightPixels[] = {2, 101, 106, 107, 108, 109, 98, 5, 4, 3};
  int leftPixels[] = {12, 91, 116, 117, 118, 119, 88, 15, 14, 13};
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
Hexgrid_fade(500); 
//Number gives timespan of effect

delay(250);
Glow(350, strip.Color(100, 100, 100), strip.Color(100, 100, 0));
//Number gives timespan of effect. Color 1 is wipe color, Color 2 is "GLOW" color
delay(1000);
}

void All_off(){
    for (int i = 0; i < 573; i++){
    strip.setPixelColor(i, strip.Color(0,0,0));
    }
}

void Hexgrid_fade(uint32_t time){
    for (int i = 0; i < 20; i++){
    Hexgrid(strip.Color(0, 0, 127*i/20));
    delay(time/20);
    }
}


void Hexgrid(uint32_t color){ 
        for (int i = 0; i < 9; i++){
	for (int j = 0; j < 6; j++){
		strip.setPixelColor((6*i+104*j+1), color);
                strip.setPixelColor((6*i+104*j+3), color);
	}
	}
        for (int i = 0; i < 9; i++){
	for (int j = 0; j < 6; j++){
		strip.setPixelColor((55+6*i+104*j), color);
                strip.setPixelColor((57+6*i+104*j), color);
	}
    	}
        strip.show();
}

void Glow(uint32_t time, uint32_t color1, uint32_t color2){
      int pixels[] = {101, 99, 97, 95, 93, 88, 86, 84, 82, 80, 75, 73, 71, 69, 67, 60, 54, 105, 115, 119, 131, 141, 146, 148, 152, 154, 206, 201, 196, 192, 180, 170, 166, 162, 160, 156, 209, 214, 216, 218, 223, 235, 245, 249, 254, 259, 310, 296, 284, 274, 270, 260, 313, 327, 339, 349, 353, 363, 414, 404, 400, 388, 378, 374, 364, 417, 427, 431, 443, 453, 457, 467, 517, 515, 513, 511, 509, 504, 491, 489, 487, 485, 483, 478, 468};
      //0-16 : Col 2
      //17-25 : Col 3
      //26-35 : Col 4
      //36-45 : Col 5
      //46-51 : Col 6
      //52-57 : Col 7
      //58-64 : Col 8
      //65-71 : Col 9
      //72-84 : Col 10
      
      //Frame 1
        for (int i = 0; i < 52; i++){
        strip.setPixelColor(i, color1);
        }
        strip.show();
        delay(time/11);
        
      //Frame 2
        for (int i = 52; i < 104; i++){
        strip.setPixelColor(i, color1);
        }
        for (int i = 0; i < 52; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        strip.show();
        delay(time/11);
        
        //Frame 3
        for (int i = 104; i < 156; i++){
        strip.setPixelColor(i, color1);
        }
        for (int i = 52; i < 104; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        for (int i = 0; i < 17; i++){
        strip.setPixelColor(pixels[i], color2);
        }
        strip.show();
        delay(time/11);
        
        //Frame 4
        for (int i = 156; i < 208; i++){
        strip.setPixelColor(i, color1);
        }
        for (int i = 104; i < 156; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        for (int i = 17; i < 26; i++){
        strip.setPixelColor(pixels[i], color2);
        }
        strip.show();
        delay(time/11);
        
        //Frame 5
        for (int i = 208; i < 260; i++){
        strip.setPixelColor(i, color1);
        }
        for (int i = 156; i < 208; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        for (int i = 26; i < 36; i++){
        strip.setPixelColor(pixels[i], color2);
        }
        strip.show();
        delay(time/11);
        
        //Frame 6
        for (int i = 260; i < 312; i++){
        strip.setPixelColor(i, color1);
        }
        for (int i = 208; i < 260; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        for (int i = 36; i < 46; i++){
        strip.setPixelColor(pixels[i], color2);
        }
        strip.show();
        delay(time/11);
      
        //Frame 7
        for (int i = 312; i < 364; i++){
        strip.setPixelColor(i, color1);
        }
        for (int i = 260; i < 312; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        for (int i = 46; i < 52; i++){
        strip.setPixelColor(pixels[i], color2);
        }
        strip.show();
        delay(time/11);
        
        //Frame 8
        for (int i = 364; i < 416; i++){
        strip.setPixelColor(i, color1);
        }
        for (int i = 312; i < 364; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        for (int i = 52; i < 58; i++){
        strip.setPixelColor(pixels[i], color2);
        }
        strip.show();
        delay(time/11);
        
        //Frame 9
        for (int i = 416; i < 468; i++){
        strip.setPixelColor(i, color1);
        }
        for (int i = 364; i < 416; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        for (int i = 58; i < 65; i++){
        strip.setPixelColor(pixels[i], color2);
        }
        strip.show();
        delay(time/11);
        
        //Frame 10
        for (int i = 468; i < 520; i++){
        strip.setPixelColor(i, color1);
        }
        for (int i = 416; i < 468; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        for (int i = 65; i < 72; i++){
        strip.setPixelColor(pixels[i], color2);
        }
        strip.show();
        delay(time/11);
        
        //Frame 11
        for (int i = 520; i < 572; i++){
        strip.setPixelColor(i, color1);
        }
        for (int i = 468; i < 520; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        for (int i = 72; i < 85; i++){
        strip.setPixelColor(pixels[i], color2);
        }
        strip.show();
        delay(time/11);
        
        //Frame 12
        for (int i = 520; i < 572; i++){
        strip.setPixelColor(i, strip.Color(0,0,0));
        }
        strip.show();
}
