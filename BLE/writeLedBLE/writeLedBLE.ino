

/*
  Arduino Nano 33 BLE Getting Started
  BLE peripheral with a simple Hello World greeting service that can be viewed
  on a mobile phone
  Adapted from Arduino BatteryMonitor example
*/

#include <ArduinoBLE.h>


BLEService ledService("842f2d25-20c3-4a94-b0cc-c30f2c03fb94"); // create service
BLEByteCharacteristic ledOnOffCharacteristic("e16034b3-85d0-4554-aa3e-0d51ce815f03", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // pin to use for the LED

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin

  if (!BLE.begin()) {   // initialize BLE
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("Nano33BLE_LED");  // Set name for connection
  
  ///// LED Service /////
  BLE.setAdvertisedService(ledService); // Advertise service
  
  // add the characteristic to the service
  ledService.addCharacteristic(ledOnOffCharacteristic);
  
  // assign event handlers for characteristic
  ledOnOffCharacteristic.setEventHandler(BLEWritten, ledOnOffCharacteristicWrittenHandler);
  
  // set initial value for the characteristic
  ledOnOffCharacteristic.setValue(0);

  // add service
  BLE.addService(ledService);

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

    while (central.connected()){
      BLE.poll();
      } // keep looping while connected
    
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central MAC: ");
    Serial.println(central.address());
  }
}

void ledOnOffCharacteristicWrittenHandler(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");
  
  if (ledOnOffCharacteristic.value()) {
    Serial.println("LED on");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("LED off");
    digitalWrite(ledPin, LOW);
  }
}
