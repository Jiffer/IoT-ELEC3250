
/*
   Modified from code at: https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/

  Example 2 - Transmitter Code by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(5, 4); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

const int ledPin = 2;

void setup() {
  // set ledPin to be an output
  pinMode(ledPin, OUTPUT);
  // init the radio 
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
  int potValue = analogRead(A0); // ESP32 pin 36
  int mappedValue = map(potValue, 0, 1023, 0, 255);
  radio.write(&mappedValue, sizeof(mappedValue));
  // Short delay while data transmits
  delay(5);

  // go back into listening mode
  radio.startListening();
  if (radio.available()) {
    while (radio.available()) {
      Serial.print("got digital: ");
      boolean buttonState = 0;
      radio.read(&buttonState, sizeof(buttonState));
      Serial.println(buttonState);
      // set led output based on receieved value
      // should be 1 or 0 (HIGH or LOW)
      digitalWrite(ledPin, buttonState);
    }
  }
  delay(5);
}
