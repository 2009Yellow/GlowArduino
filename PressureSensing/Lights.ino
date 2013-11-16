// Light Constants
#define STRIP_PURPLE (strip.Color(30, 0, 60))
#define STRIP_RED (strip.Color(120, 30, 30))
#define STRIP_WHITE (strip.Color(30, 30, 30))

// Mat light data
int lightPositions [4];
int lightColors[4];

void initLights() {
  // Initialize the light pins
  // Initialize the light state
}

void recieveLightData() {
  char firstChar = Serial.read(); 
  if (firstChar != SERIAL_LIGHT_FIRST_RECEIVE_CHAR) {
    Serial.write(SERIAL_LIGHT_ERROR_CHAR);
    return;
  }
  
  // Read position values
  for (int i = 0; i < 4; ++i) {
    int value = 0;
    // Read high byte
    value += ((int)Serial.read()) << 8;
    // Read low byte
    value += ((int)Serial.read());
    // Update value
    lightPositions[i] = value;
  }
  
  // Read color values
  for (int i = 0; i < 4; ++i) {
    lightColors[i] = ((int)Serial.read());
  }
  
  char finalChar = Serial.read();
  if (finalChar != SERIAL_LIGHT_FIRST_RECEIVE_CHAR) {
    Serial.write(SERIAL_LIGHT_ERROR_CHAR);
    return;
  }
}

void updateLights() {
  // Update the lights with data
}
