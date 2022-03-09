
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

WiFiClient wifiClient;

const char* ssid = "IoT_External";
const char* password = "CU!IoT#303315776Ex";

void setup () {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);
    Serial.print("Connecting..");

  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin(wifiClient, "http://jsonplaceholder.typicode.com/users/1");  //Specify request destination
    int httpCode = http.GET();                                  //Send the request

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload

      ////// TODO Deserialize: //////////////////////////////
      ////// https://arduinojson.org/v6/assistant/ //////////
      ///////////////////////////////////////////////////////
      

      ////// End Code from: /////////////////////////////////
      ////// https://arduinojson.org/v6/assistant/ //////////
      ///////////////////////////////////////////////////////

      // TODO: Parsing
      // select one of the object keys and print it out
      // use something other than the example given in the lab (phone)

    }


    http.end();   //Close connection

  }
  delay(30000);    //Send a request every 30 seconds
}
