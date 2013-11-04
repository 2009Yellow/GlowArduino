// Mat constants
const int WIDTH = 16;  // Sense pints
const int HEIGHT = 16;  // Drive pins
const int MAT_SIZE = WIDTH * HEIGHT;

// Serial Communcation Constants
const char SERIAL_START_CHAR = 'A';
const char FIRST_SERIAL_RECEIVE_CHAR = 'B'; 
const char FINAL_SERIAL_RECEIVE_CHAR = 'C';

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
const int SENSE_MUX0_ADDR0 = 30;
const int SENSE_MUX0_ADDR1 = 31;
const int SENSE_MUX0_ADDR2 = 32;
const int SENSE_MUX0_ADDR3 = 33;


// Mat pressure data
int adcValues [MAT_SIZE];

void setup() {
  // Init all digital and analog pins
  initPins();
  // Init mux state
  disableDriveAndSense();
  // Init  serial port at 9600 bps
  Serial.begin(9600);
}

void initPins() {
  // Initialize drive pins
  pinMode(DRIVE_MUX_EN, OUTPUT);      
  pinMode(DRIVE_MUX_ADDR0, OUTPUT);      
  pinMode(DRIVE_MUX_ADDR1, OUTPUT);      
  pinMode(DRIVE_MUX_ADDR2, OUTPUT);      
  pinMode(DRIVE_MUX_ADDR3, OUTPUT);      

  // Initialize sense pins
  pinMode(SENSE_MUX0_EN, OUTPUT);      
  pinMode(SENSE_MUX0_ADDR0, OUTPUT);      
  pinMode(SENSE_MUX0_ADDR1, OUTPUT);      
  pinMode(SENSE_MUX0_ADDR2, OUTPUT);      
  pinMode(SENSE_MUX0_ADDR3, OUTPUT);  
}


void loop(){
  // Main process loop
  while(true) {
    // Read ADC inputs
    processMat();
  }
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
    char input = (char)Serial.read(); 
    
    // Only start sending values if receive start char
    if (input != SERIAL_START_CHAR) {
        return;
    }
    
    // Send start char of sequence
    Serial.write(FIRST_SERIAL_RECEIVE_CHAR);

    // Send payload data
    for (int i = 0; i < MAT_SIZE; ++i) {
        Serial.write(adcValues[i]);
    }

    // Send final char of sequence
    Serial.write(FINAL_SERIAL_RECEIVE_CHAR);
  } // end serial available
}


void putHalfByte(int data, int startAddr){
  byte mask;
  int addr;
  for (int i = 0; i < 4; ++i) {
    mask = 0x01 << i;  // Get bitmask
    addr = startAddr + i;
    if (data & mask) {
      digitalWrite(addr,HIGH);
    } else {
      digitalWrite(addr,LOW);
    }
  }
}


void processMat() {
  for (int j = 0; j < HEIGHT ; ++j) {
    for (int i = 0; i < WIDTH; ++i) {
      // Setup mat pins
      configureMat(i,j);
      // Read ADC value
      adcValues[j * WIDTH + i] = analogRead(ADC_IN_PIN);
    }
  }
}


void configureMat(int i, int j) {
  // Disable AND MUX drive during switch
  disableDriveAndSense();
  // Set drive address
  putHalfByte(j, DRIVE_MUX_ADDR0);
  // Set sense address
  int senseMuxSel = i / 16;
  switch (senseMuxSel) {
    case 0:
      putHalfByte(i, SENSE_MUX0_ADDR0);
      digitalWrite(SENSE_MUX0_EN, ENABLE);
      break;
  }
  // Enable drive now switch is complete
  enableDrive();

}


void enableDrive() {
  // Disable drive and fet muxes (grounds all drive pins)
  digitalWrite(FET_MUX_EN, ENABLE);
  digitalWrite(DRIVE_MUX_EN, ENABLE);
}


void disableDriveAndSense() {
  // Disable drive and fet muxes (grounds all drive pins)
  digitalWrite(DRIVE_MUX_EN, DISABLE);
  digitalWrite(FET_MUX_EN, DISABLE);
  // Disable all sense muxes
  digitalWrite(SENSE_MUX0_EN, DISABLE);
}
