/*
   Modified version of code at: https://howtomechatronics.com/tutorials/arduino/how-to-build-an-arduino-wireless-network-with-multiple-nrf24l01-modules/

  Arduino Wireless Network - Multiple NRF24L01 Tutorial
        == Node 02 (Child of Master node 00) ==

        // Sends a message to node01
       
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
RF24 radio(0, 2);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
#include <ESP32Servo.h>

const uint16_t this_node = 02;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;
const uint16_t master00 = 00;    // Address of the other node in Octal format

const unsigned long interval = 10;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?

// digital inputs
const int button1 = 16; // controls LED on node 00
const int button2 = 17; // controls LED on node 01
// output
// LEDC (LED Control) parameters
//int ledPin = 25;
//int ledChannel = 0;

// Servo stuff
int servoPin = 25;
Servo myServo;

void setup() {
  // init radio and network:
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_1MBPS);

  // set up pins
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  // start serial
  Serial.begin(9600);
  Serial.println("");
  Serial.print("node 02. Listening for message from base node 00.");

  // initialize ledc channel, rate, bits
  //  ledcSetup(ledChannel, 5000, 8); // 5000 Hz, 8 bits [0:255]
  //  ledcAttachPin(ledPin, ledChannel);
  // OR with servo
  myServo.setPeriodHertz(50);// Standard 50hz servo
  myServo.attach(servoPin, 500, 2400);
}
void loop() {
  // keep messages moving
  network.update();

  //===== Sending =====//

  // check time
  unsigned long now = millis();
  if (now - last_sent >= interval) {   // If it's time to send a data, send it!
    last_sent = now; // capture the current value of millis()

    // check digital inputs
    unsigned long buttonVal1 = digitalRead(button1);
    unsigned long buttonVal2 = digitalRead(button2);

    // send to node 00
    RF24NetworkHeader header(master00);
    bool ok = network.write(header, &buttonVal1, sizeof(buttonVal1)); // Send the data
    // send to node 01
    RF24NetworkHeader header2(node01);
    bool ok2 = network.write(header2, &buttonVal2, sizeof(buttonVal2)); // Send the data
  }

  // set brightness of LED (LEDC) or servo angle
  //===== Receiving =====//
  // check for incoming data
  while ( network.available() ) {
    RF24NetworkHeader header;
    unsigned long incomingData;

    // read data and header
    network.read(header, &incomingData, sizeof(incomingData));

    // check where the data came from
    if (header.from_node == 00) {    // If data comes from Node 00
      //digitalWrite(led, incomingData);
      Serial.print("message from node00: ");
      Serial.println(incomingData);

      // incoming analog data from base00 is
      // expected to be between 0:4095
      
      // map to 0:255 for LED brightness control
      // int mappedValue = map(incomingData, 0, 4095, 0, 255);
      // ledcWrite(ledChannel, mappedValue);

      // map input to appropriate range for servo
      int mappedValue = map(incomingData, 0, 1023, 0, 180);
      myServo.write(mappedValue);
    }
  }
}
