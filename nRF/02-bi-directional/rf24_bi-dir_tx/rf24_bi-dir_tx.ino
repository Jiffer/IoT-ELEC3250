
/*
   Modified from code at: https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/

  Example 2 - Transmitter Code by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <nRF24L01.h>
#include <RF24.h>

// TODO: PART 2 
// add code to control an LED output

RF24 radio(5, 4); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

void setup() {
//  init the radio
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  // power levels: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(2);  // number between 0-124

  Serial.begin(9600);
  Serial.println("Arduino 1");
}


// bi-directional example
void loop() {
  // go into transmit mode mode
  radio.stopListening(); // cannot listen and write at the same time
  int potValue = analogRead(A0); // 
  int mappedValue = map(potValue, 0, 1023, 0, 255); // esp8266 10 bit ADC
  radio.write(&mappedValue, sizeof(mappedValue));
  // Short delay while data transmits
  delay(5);

  // TODO: PART 2 
  // Go into listening mode
  // check for radio input 
  // if a 1 is received turn LED on
  // if a 0 is received turn LED off
  
  delay(5);
}
