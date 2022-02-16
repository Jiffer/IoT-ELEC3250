/* 
 * Modified version of code at: https://howtomechatronics.com/tutorials/arduino/how-to-build-an-arduino-wireless-network-with-multiple-nrf24l01-modules/
 * 
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
          == Base/ Master Node 00==
  by Dejan, www.HowToMechatronics.com
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
*/

#include <RF24Network.h>
#include <RF24.h>


const int button = 16;
const int led = 10;

RF24 radio(5, 4);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node02 = 02;
//const uint16_t node012 = 012;
//const uint16_t node022 = 022;

void setup() {
  SPI.begin();
  radio.begin();
  // set channel and this node's address
  network.begin(90, this_node);  
  radio.setDataRate(RF24_1MBPS);
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial.println(" base node 00, listening for message from  node 02 sending to 01");
}

void loop() {
  network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long incomingData;
    Serial.println(incomingData);
    // read incoming data
    network.read(header, &incomingData, sizeof(incomingData)); 
    digitalWrite(led, incomingData);

  }
  //===== Sending =====//
  // brightness control at Node 02
  unsigned long potValue = analogRead(A0);
  RF24NetworkHeader header2(node02);     // (Address where the data is going)
  network.write(header2, &potValue, sizeof(potValue)); // Send the data

}
