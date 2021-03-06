// Mat constants
const int WIDTH = 16;  // Sense pints
const int HEIGHT = 16;  // Drive pins
const int MAT_SIZE = WIDTH * HEIGHT;

// Serial Communcation Constants
const int SERIAL_ESTABLISH_CONTACT_CHAR = 'A';

const int SERIAL_PRESSURE_START_CHAR = 'B';
const int SERIAL_PRESSURE_FIRST_RECEIVE_CHAR = 'C'; 
const int SERIAL_PRESSURE_FINAL_RECEIVE_CHAR = 'D';

const int SERIAL_LIGHT_START_CHAR = 'E';
const int SERIAL_LIGHT_FIRST_RECEIVE_CHAR = 'F'; 
const int SERIAL_LIGHT_FINAL_RECEIVE_CHAR = 'G';
const int SERIAL_LIGHT_ERROR_CHAR = 'H';
const int SERIAL_LIGHT_FINISHED_UPDATING_CHAR = 'I';

const int SERIAL_STARTUP_CHAR = 'J';
const int SERIAL_FINISHED_STARTUP_CHAR = 'K';

//const int SERIAL_LIGHT_START_CHAR = 'E';
//const int SERIAL_LIGHT_FIRST_RECEIVE_CHAR = 'F'; 
//const int SERIAL_LIGHT_SWITCH_TO_COLOR_CHAR = 'G';
//const int SERIAL_LIGHT_FINAL_RECEIVE_CHAR = 'H';
//const int SERIAL_LIGHT_ERROR_CHAR = 'I';

// Used to randomize drive pin outputs
static int DRIVE_JUMP_TAB[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

// Enable/Disable constatnts
const boolean ENABLE = LOW;
const boolean DISABLE = HIGH;

// Drive pin number constants
const int DRIVE_MUX_EN = 2;
const int DRIVE_MUX_ADDR0 = 22;
const int DRIVE_MUX_ADDR1 = 23;
const int DRIVE_MUX_ADDR2 = 24;
const int DRIVE_MUX_ADDR3 = 25;
const int FET_MUX_EN = 3;

// Sense pin number constants
const int ADC_IN_PIN = A0;
const int SENSE_MUX0_EN = 4;
const int SENSE_MUX0_ADDR0 = 26;
const int SENSE_MUX0_ADDR1 = 27;
const int SENSE_MUX0_ADDR2 = 28;
const int SENSE_MUX0_ADDR3 = 29;

// Mat pressure data
int adcValues [MAT_SIZE];



void setup() {
  // Init all digital and analog pins
  initPins();
  // Init the lights
  initLights();
  // Init mux state
  disableDrive();
  // Init  serial port at 9600 bps
  Serial.begin(115200);
  establishContact();
  // Enable all analog pins
  digitalWrite(SENSE_MUX0_EN, HIGH);
  //drawPressureButtons();
}


void initPins() {
  // Initialize drive pins
  pinMode(DRIVE_MUX_EN, OUTPUT);      
  pinMode(DRIVE_MUX_ADDR0, OUTPUT);      
  pinMode(DRIVE_MUX_ADDR1, OUTPUT);      
  pinMode(DRIVE_MUX_ADDR2, OUTPUT);      
  pinMode(DRIVE_MUX_ADDR3, OUTPUT); 
  pinMode(FET_MUX_EN, OUTPUT);

  // Initialize sense pins
  pinMode(SENSE_MUX0_EN, OUTPUT);      
  pinMode(SENSE_MUX0_ADDR0, OUTPUT);      
  pinMode(SENSE_MUX0_ADDR1, OUTPUT);      
  pinMode(SENSE_MUX0_ADDR2, OUTPUT);      
  pinMode(SENSE_MUX0_ADDR3, OUTPUT);
}


void establishContact() {
  while (Serial.available() <= 0) {
    Serial.write(SERIAL_ESTABLISH_CONTACT_CHAR);   // send a capital A
    delay(300);
  }
  
 /* startSequence();
  delay(1000);
  startSequence();
  delay(1000);
  pixelsOff();*/
  
  
}


void loop(){
  // Read ADC inputs
  
  processMat();
}


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char input = Serial.read(); 
    //Serial.println("serialEvent:" + String(input));
    
    // Only start sending values if receive start char
    if (input == SERIAL_PRESSURE_START_CHAR) {
      sendPressureData();
    } 
    else if (input == SERIAL_LIGHT_START_CHAR) {
      receiveLightData();
      updateLights();
      Serial.write(SERIAL_LIGHT_FINISHED_UPDATING_CHAR);
    } 
    
    else if (input == SERIAL_STARTUP_CHAR){
      startSequence();
      Serial.write(SERIAL_FINISHED_STARTUP_CHAR);
    }
    else {
      // Do nothing
      continue; 
    }
  } // end serial available
}



void sendPressureData() {
  // Send start char of sequence
  Serial.write(SERIAL_PRESSURE_FIRST_RECEIVE_CHAR);

  // Send payload data
  for (int i = 0; i < MAT_SIZE; ++i) {
    Serial.write(adcValues[i]);
  }

  // Send final char of sequence
  Serial.write(SERIAL_PRESSURE_FINAL_RECEIVE_CHAR);
}



void serialInterrupt() {
  if (Serial.available()) {
    serialEvent();
  }
}


void putHalfByte(int data, int startAddr){
  byte mask;
  int addr;
  //Serial.println("putHalfByteData " + String(data));
  for (int i = 0; i < 4; ++i) {
    mask = 0x01 << i;  // Get bitmask
    addr = startAddr + i;
    //Serial.println("byte " + String(i) + " data " + (data & mask) + " at addr " + addr);
    if (data & mask) {
      digitalWrite(addr,HIGH);
    } 
    else {
      digitalWrite(addr,LOW);
    }
  }
}


void processMat() {
  for (int j = 0; j < HEIGHT ; ++j) {
    for (int i = 0; i < WIDTH; ++i) {
      int jump_j = DRIVE_JUMP_TAB[j];
      serialInterrupt();
      // Setup mat pins
      configureMat(i,jump_j);
      // Read ADC value
      adcValues[jump_j * WIDTH + i] = analogRead(ADC_IN_PIN) >> 2;
      //delay(10);
      //adcValues[j * WIDTH + i] = analogRead(ADC_IN_PIN) >> 2;
    }
  }
}


void configureMat(int i, int j) {
  // Disable AND MUX drive during switch
  disableDrive();
  // Set drive address
  putHalfByte(j, DRIVE_MUX_ADDR0);
  // Set sense address
  int senseMuxSel = i / 16;
  switch (senseMuxSel) {
  case 0:
    putHalfByte(i, SENSE_MUX0_ADDR0);
    //digitalWrite(SENSE_MUX0_EN, ENABLE);
    break;
  }
  // Enable drive now switch is complete
  enableDrive();
}


void enableDrive() {
  // Disable drive and fet muxes (grounds all drive pins)
  digitalWrite(FET_MUX_EN, HIGH);
  digitalWrite(DRIVE_MUX_EN, HIGH);
}


void disableDrive() {
  // Disable drive and fet muxes (grounds all drive pins)
  digitalWrite(DRIVE_MUX_EN, LOW);
  digitalWrite(FET_MUX_EN, LOW);

}

