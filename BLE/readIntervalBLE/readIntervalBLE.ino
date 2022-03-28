

/*
  Arduino Nano 33 BLE Getting Started
  BLE peripheral with a simple Hello World greeting service that can be viewed
  on a mobile phone
  Adapted from Arduino BatteryMonitor example
*/

#include <ArduinoBLE.h>



BLEService sensorService("478acafe-8b6c-4438-848b-c258486fa14f");  // User defined service

const int ledPin = LED_BUILTIN; // pin to use for the LED

BLEIntCharacteristic intSensorCharacteristic("e42383c0-e0b3-44ed-b4dc-9a9d2ef864b1", BLERead);

unsigned long sensorTimer = 0;
int sensorInterval = 20000; // 20 second update interval
int sensorVal = -1;

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin

  if (!BLE.begin()) {   // initialize BLE
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("sensorNano33BLE");  // Set name for connection
  BLE.setAdvertisedService(sensorService); // Advertise service
  
  sensorService.addCharacteristic(intSensorCharacteristic);
  
  // Add service
  BLE.addService(sensorService); 
  // Set greeting characteristics initial values
  intSensorCharacteristic.writeValue(sensorVal);

  BLE.advertise();  // Start advertising
  Serial.print("Peripheral device MAC: ");
  Serial.println(BLE.address());
  Serial.println("Waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();  // Wait for a BLE central to connect

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central MAC: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);


    while (central.connected()) {
      // Check for new BLE data and service it with any handlers
      BLE.poll();

      // Don't use Delay, use a timer so the poll() function
      // can be called to service any incoming requests
      if (millis() > sensorTimer) {
        sensorTimer = millis() + sensorInterval;
        // 1.3.4 TODO: replace analogRead with onboard sensor reading
        // 1.4 TODO: add another characteristic with BLENotify parameter for a 2nd sensor
        sensorVal = analogRead(A0);
        intSensorCharacteristic.writeValue(sensorVal);
        
      }
    } // keep looping while connected


    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central MAC: ");
    Serial.println(central.address());
  }
}
