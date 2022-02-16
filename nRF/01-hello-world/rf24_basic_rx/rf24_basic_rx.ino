/*
 * Modified from code at: https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
* Arduino Wireless Communication Tutorial
*     Example 2 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <nRF24L01.h>
#include <RF24.h>
//#include <Servo.h>


//int button = 4;
//int led = 5;

RF24 radio(5, 4); // CE, CSN
const byte addresses[6] = {"00001"};
boolean buttonState = 0;

void setup() {
//  pinMode(button, INPUT_PULLUP);
//  pinMode(led, OUTPUT);
  
  radio.begin();
  radio.openReadingPipe(1, addresses); 
    // power levels: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(99);  // number between 0-124
  radio.startListening();

  Serial.begin(9600);
  Serial.println("");
  Serial.println("starting main loop... ");
}

void loop() {
  delay(5);
  
  if ( radio.available()) {
    while (radio.available()) {
      char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    }
    
  }
}
