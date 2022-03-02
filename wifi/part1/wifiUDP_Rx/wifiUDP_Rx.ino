//
// wifiUDP_Rx
//
// Jiffer Hariman
//
// UDP transmit and receive example.
//
// parses comma delimited string message
//
// The static IP address of this device needs to be unique (relative to others on the network)
// The remoteIP address list includes other devices that the UDP messages will be sent to.

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#define UDP_TX_PACKET_MAX_SIZE 860 

// WiFi variables
const char *ssid = "things";
const char *password = "connected";

boolean wifiConnected = false;

// my IP requested
IPAddress ip(192,168,86,53);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// UDP variables
unsigned int localPort = 8000;
WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,

// output pins
// int ledPin = 5;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // connect to wifi
  // Initialise wifi connection
  wifiConnected = connectWifi();

  // only proceed if wifi connection successful
  if (wifiConnected) {
    Serial.println();

    Serial.print("Hooray! Connected to ");
    Serial.println(ssid);
    Serial.print("I'm using IP address: ");
    Serial.println(WiFi.localIP());

    // initialize output pins to control the outputs
//    pinMode(ledPin, OUTPUT);
  }

  udpConnected = connectUDP();
  if (udpConnected) {
    // congrats
    Serial.println("udp connected");
  }
}

void loop() {
  
  // check if the WiFi and UDP connections were successful
  if (wifiConnected) {
    if (udpConnected) {
      
      // if there’s data available, read a packet
      int packetSize = UDP.parsePacket();
      if (packetSize > 0) 
      {
        Serial.println("");
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = UDP.remoteIP();
        for (int i = 0; i < 4; i++)
        {
          Serial.print(remote[i], DEC);
          if (i < 3)
          {
            Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(UDP.remotePort());

        // read the packet into the bufffer
        UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

        // Convert received data into an int
        int receivedValue = packetBuffer[0] * 256 + packetBuffer[1];
        Serial.print("got message! as int: ");
        Serial.println(receivedValue);
        // print as String
        Serial.print("as string: ");
        Serial.println(String((char*)packetBuffer));
      }
    }
  }
}

// connect to UDP – returns true if successful or false if not
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

//////////////////////////////
// connect to wifi – returns true if successful or false if not
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
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
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
