#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

/* WiFi access point settings. Update them as needed */
const char *ssid = "IoT_External";
const char *password = "CU!IoT#303315776Ex";

// web page to connect to and print to Serial:
const char* host = "www.example.com";

// initialization
void setup()
{
  // Start serial (note baud rate!)
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  
  // wait while board connects to WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // after successful connection, print these details
  Serial.println(" connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// *****************
// main
// *****************
void loop()
{
  WiFiClient client;

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, 80))
  {
    Serial.println("connected]");

    Serial.println("[Sending a request]");
    client.print(String("GET /") + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );

    Serial.println("[Response:]");
    while (client.connected() || client.available())
    {
      if (client.available())
      { // print one line at a time
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    } 
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
  }
  delay(5000);
}
