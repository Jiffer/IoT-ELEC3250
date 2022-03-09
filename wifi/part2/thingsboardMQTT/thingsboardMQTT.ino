
#include <ESP8266WiFi.h>
#include <ThingsBoard.h>

#define WIFI_AP "things"
#define WIFI_PASSWORD "connected"

#define TOKEN "YOUR_TOKEN_HERE"


char thingsboardServer[] = "demo.thingsboard.io";

WiFiClient wifiClient;

ThingsBoard tb(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  
  delay(10);
  InitWiFi();
  lastSend = 0;
}

void loop()
{
  if ( !tb.connected() ) {
    reconnect();
  }

  if ( millis() - lastSend > 10000 ) { // Update and send after 10 seconds
    readSensorAndSendData();
    lastSend = millis();
  }

  tb.loop();
}

void readSensorAndSendData()
{
  Serial.println("Collecting  data.");

  // Generate a random number for temperature and humidity...
  float humidity = random(10000) / 100.0;
  float temperature = random(1000)/100.0;
  int randomVal = random(100);

  // TODO: read a pin on the Arduino and send the value to thingsboard
  // may be digital or analog input.
  // create a widget in thingsboard to display the value

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read valid data");
    return;
  }

  Serial.println("Sending data to ThingsBoard:");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C ");
  Serial.print("Wacky: ");
  Serial.println(randomVal);
  


  // send MQTT key/data pairs 
  tb.sendTelemetryFloat("temperature", temperature);
  tb.sendTelemetryFloat("humidity", humidity);
  tb.sendTelemetryInt("wacky", randomVal);
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!tb.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    if ( tb.connect(thingsboardServer, TOKEN) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED]" );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
