// Modified version of Seth's WiFire demo code that uses I2C.
// Assumes that indices are being sent as a block of bytes with the
// following format:
// [address][command][index1][index2] ...

#include <SPI.h>
#include <Wire.h>

// pin definitions
#define CLEARPIN 4    // master clear for 74HC595 shift registers
#define LATCHPIN 5    // latch for 74HC595 shift registers
#define OEPIN    6    // output enable for 74HC595 shift registers
#define ARMEDPIN 7    // optoisolator connected to load power
#define DATAPIN  11   // data for 74HC595 shift registers
#define CLOCKPIN 13   // clock for 74HC595 shift registers

// for i2c
#define SLAVE_ADDRESS 0x04 

#define bitFlip(x,n)  bitRead(x,n) ? bitClear(x,n) : bitSet(x,n)

byte c;
byte r1 = 0, r2 = 0;

void setup() {
  // set up I2C
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  // set all output pins
  SPI.begin(); // handles DATAPIN and CLOCKPIN
  pinMode(LATCHPIN, OUTPUT);
  pinMode(OEPIN, OUTPUT);
  pinMode(CLEARPIN, OUTPUT);

  // make sure no lines go active until data is shifted out
  digitalWrite(CLEARPIN, HIGH);
  digitalWrite(OEPIN, LOW);

  // clear any lines that were left active
  sendSPI(0,0);  
  
  // activate built-in pull-up resistor 
  digitalWrite(ARMEDPIN, HIGH);
}

void loop() {
    delay(100);
}

void sendSPI(byte one, byte two) {
  digitalWrite(LATCHPIN, LOW);
  digitalWrite(OEPIN, HIGH);
  c = SPI.transfer(two);
  c = SPI.transfer(one);
  digitalWrite(LATCHPIN, HIGH);
  digitalWrite(OEPIN, LOW);
}

// callback for received data
void receiveData(int byteCount){
  c = 0;
  Wire.read(); // skip address byte
  Wire.read(); // skip command byte
  while(Wire.available()) {
    c = Wire.read();
    switch(c) {
      case 0x0 : bitFlip(r1,0); break;
      case 0x1 : bitFlip(r1,1); break;
      case 0x2 : bitFlip(r1,2); break;
      case 0x3 : bitFlip(r1,3); break;
      case 0x4 : bitFlip(r1,4); break;
      case 0x5 : bitFlip(r1,5); break;
      case 0x6 : bitFlip(r1,6); break;
      case 0x7 : bitFlip(r1,7); break;
      case 0x8 : bitFlip(r2,0); break;
      case 0x9 : bitFlip(r2,1); break;
      case 0xA : bitFlip(r2,2); break;
      case 0xB : bitFlip(r2,3); break;
      case 0xC : bitFlip(r2,4); break;
      case 0xD : bitFlip(r2,5); break;
      case 0xE : bitFlip(r2,6); break;
      // we aren't using the 16th solenoid, so repurposing it as an all-off signal
      case 0xF : r1 = 0; r2 = 0; break; 
    }
  }
  sendSPI(r1, r2);
}

// callback for sending data (for debugging only)
void sendData(){
    Wire.write(c);
}
