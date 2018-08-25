#include <OneWire.h> // https://www.pjrc.com/teensy/td_libs_OneWire.html

OneWire  ds(10);
byte addr[8];
String keyStatus="";

// I'm using a DS9092L+ reader, with four leads (red,white,black,green)

// green is data
// red is ground
// Pin 10 is D10
// 4.7K pull-up resistor pulling pin 10 to 5V+
// LED is connected. White lead is ground, Black lead is power.
// I'm using a 10k and a 100r resistor in parallel to make a 1k

int ledPin = 17; // A3

void setup(void) {
  Serial.begin(115200);
  pinMode(17, OUTPUT);
  digitalWrite(ledPin, LOW);
}


void loop(void) {
  digitalWrite(ledPin, LOW);
  getKeyCode();
  if(keyStatus=="ok"){
      byte i;
      for( i = 5; i >0; i--) {
           Serial.print(":");
           Serial.print(addr[i], HEX);           
      }
  }
  else //if (keyStatus!="") 
  {
    Serial.print(keyStatus);
  }
  
  delay(1000);
}


void getKeyCode(){
  byte present = 0;
  byte data[12];
  keyStatus="nothing\n";
  
  // Search for the next device. The addrArray is an 8 byte array. 
  // If a device is found, addrArray is filled with the device's address and true is returned. 
  // If no more devices are found, false is returned.
  bool foundDevice = ds.search(addr);
  if ( !foundDevice) {
      // No device...
      Serial.println("No device..");
      ds.reset_search();
      return;
  }else{
      Serial.println("Device!");
  }    
  digitalWrite(ledPin, HIGH);

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      keyStatus="CRC invalid";
      return;
  }
  
  if ( addr[0] != 0x01) {
      keyStatus="not DS1990A";
      return;
  }
  keyStatus="ok";
  ds.reset();
}
