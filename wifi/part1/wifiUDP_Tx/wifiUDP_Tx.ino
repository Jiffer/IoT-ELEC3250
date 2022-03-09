//
// wifiUDP_Tx
//
// Jiffer Hariman
//
//
// UDP transmit and receive example.
//
// Sends a counter that increments from 0-16 and starts over
// When digital input GPIO5 (nodeMCU pin labelled D1) it sends a one time string message
//
// The static IP address of this device needs to be unique (relative to others on the network)
// The remoteIP address list includes other devices that the UDP messages will be sent to.

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#define UDP_TX_PACKET_MAX_SIZE 860

// WiFi variables
const char *ssid = "IoT_External";
const char *password = "CU!IoT#303315776Ex";
boolean wifiConnected = false;

// My IP address:
// TODO : change the IP address requested to match the one used for the rest of the lab
// You may need to change this depending the network you use
IPAddress ip(192, 168, 86, 59);

IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);


const int buttonPin = 0;   // GPIO 5 = nodeMCU D1

// keep track of button state to check if it has changed
int buttonState;
int lastButtonState;

// this number will increment and be sent to the remote IP address list
int beat = 0;

// timer variables
unsigned long lastSend = 0;
int interval = 2000; // every 2 seconds

// IP addresses to send to:
//IPAddress remoteIPAddress =  IPAddress(192, 168, 86, 58); // laptop
IPAddress remoteIPAddress =  IPAddress(10,20,128,136);   // ESP8266

// UDP variables
WiFiUDP UDP;
unsigned int localPort = 8000;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,

// char array (string) to send
char  msgBuffer[] = "hello out there!";   // absolute max of 8192 bytes

// *************************
// Initialization
// *************************
void setup() {
  // Initialise Serial connection
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  // only proceed if wifi connection successful
  if (wifiConnected) {
    udpConnected = connectUDP();
    if (udpConnected) {
      Serial.print("wifiUDP_Tx, Connected to: ");
      Serial.println(ssid);

      // initialize pins
      pinMode(buttonPin, INPUT_PULLUP);
    }
  }
}

// *************************
// main loop
// *************************
void loop() {

  // only send if the WiFi and UDP connections were successful
  if (wifiConnected) {
    if (udpConnected) {

      // check button
      buttonState = digitalRead(buttonPin);

      // if the button has changed from HIGH to LOW
      // UDP message 2:
      if (buttonState == LOW && buttonState != lastButtonState) {
        // when button is pressed, send a message to the first IP address onlye:
        Serial.println("button pressed, sending message");
        UDP.beginPacket(remoteIPAddress, localPort);
        UDP.printf(msgBuffer);
        int success = UDP.endPacket();
      }

      // save button state to compare next time
      lastButtonState = buttonState;

      // Timer: only send after "interval"
      // milliseconds since the last send
      if (millis() > (lastSend + interval)) {
        // reset timer
        lastSend = millis();

        // UDP message 1: only send after interval
        Serial.print("sending number:");
        Serial.println(beat);
        // Only sending a 2 byte message here
        // to represent up to a 16 bit integer
        byte message[2];
        message[0] = 0;
        message[1] = beat++;
        beat %= 255;           // 2 bytes can hold integer value up to  2^16 = 65,535

        // broadcast this value to all the IP Addresses in the list:

        UDP.beginPacket(remoteIPAddress, localPort);
        UDP.write(message, sizeof(message));
        int success = UDP.endPacket();

      }
    }
  }
}


// *************************
// connect to UDP – returns true if successful or false if not
// *************************
boolean connectUDP() {
  boolean state = false;

  Serial.println("");
  Serial.println("Connecting to UDP");

  if (UDP.begin(localPort) == 1) {
    Serial.println("Connection successful");
    state = true;
  }
  else {
    Serial.println("Connection failed");
  }

  return state;
}


// *************************
// connect to wifi – returns true if successful or false if not
// *************************
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
    Serial.print("Try ");
    Serial.println(i);
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}

void udpReset() {
  // asm volatile ("  jmp 0");
  if (wifiConnected) {
    udpConnected = connectUDP();
    if (udpConnected) {
      Serial.print(" ...success");
    }
    else {
      Serial.print(" on nose!");

    }
  }
}

//////////////////////////////////////////
