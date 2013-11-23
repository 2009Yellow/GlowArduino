#include "LPD8806.h"
#include "SPI.h"

// Simple test for 160 (5 meters) of LPD8806-based RGB LED strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 572;

// Chose 2 pins for output; can be any valid output pins:
int dataPin = 6;
int clockPin = 7;

// First parameter is the number of LEDs in the strand. The LED strips
// are 32 LEDs per meter but you can extend or cut the strip. Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters. But this does limit use to very
// specific pins on the Arduino. For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13. For Arduino Mega, data = pin 51,
// clock = pin 52. For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1. For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);

void setup() {
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
}

void loop() {
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

