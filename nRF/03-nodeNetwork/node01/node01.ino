/*
   Modified version of code at: https://howtomechatronics.com/tutorials/arduino/how-to-build-an-arduino-wireless-network-with-multiple-nrf24l01-modules/
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
        == Node 01 (Child of Master node 00) ==

  // Node 01 listens for a message from Node 2
  // the incoming value is used to turn on/off 
  // pin 5 (built in LED)

*/


#include <RF24Network.h>
#include <RF24.h>

RF24 radio(5, 4);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format

// led output
const int led = 2;

// initialization:
void setup() {
  SPI.begin();
  radio.begin();
  // set channel and this node's address
  network.begin(90, this_node); //(channel, node address)
  radio.setDataRate(RF24_1MBPS);
  pinMode(led, OUTPUT);

  // start serial comms
  Serial.begin(9600);
  Serial.println("");
  Serial.println("listening for message from node 02...");
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);

}
void loop() {
  // keep messages moving
  network.update();

  //===== Receiving =====//
  // check for incoming data
  while ( network.available() ) {
    RF24NetworkHeader header;
    unsigned long incomingData;

    // read data and header
    network.read(header, &incomingData, sizeof(incomingData));

    // check where the data came from
    if (header.from_node == 02) {    // If data comes from Node 02
      digitalWrite(led, incomingData);
      Serial.println("message from node 02");
    }
  }

  // Could also send stuff to node 00 and node 02 here.
}
