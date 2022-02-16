/*
   Modified from code at: https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
  Arduino Wireless Communication Tutorial
      Example 2 - Receiver Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <nRF24L01.h>
#include <RF24.h>
//#include <Servo.h>

int buttonPin = 16; // LED interferes with INPUT_PULLUP (use external pullup/pulldown)
int buttonLed = 2;

// LED setup
int ledPin = 10;

// declare RF24 object
RF24 radio(5, 4); // radio(CE, CSN)
const byte addresses[][6] = {"00001", "00002"};
boolean buttonState = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buttonLed, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  // power levels: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(2);  // number between 0-124

  Serial.begin(9600);
  Serial.println("Arduino 2");
}

void loop() {
  delay(5);
  radio.startListening();
  if ( radio.available()) {
    while (radio.available()) {
      int dataIn = 0;
      radio.read(&dataIn, sizeof(dataIn));
      Serial.println(dataIn);
      
      // PWM control of ESP 32 pins using ledc
      // ledcWrite(ledChannel, dataIn);
      analogWrite(ledPin, dataIn);
    }

    delay(5);
    // going into Transmit mode
    radio.stopListening();
    // send the digital input pin value
    buttonState = digitalRead(buttonPin);
    digitalWrite(buttonLed, buttonState);
    radio.write(&buttonState, sizeof(buttonState));
  }
}
