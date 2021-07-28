#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <driver/adc.h>

BLECharacteristic *pCharacteristic;

#define POT_PIN 34
#define SWITCH_PIN 0


// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  uint8_t trigger;
  uint16_t pot;
  uint16_t battery;
} struct_message;

// Create a struct_message called myData
struct_message myData;


// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() {

	Serial.begin(115200);
	Serial.println("Starting BLE work!");

  pinMode(SWITCH_PIN, INPUT_PULLUP);
  // pinMode(LED_PIN, OUTPUT);

	BLEDevice::init("ESP");
	BLEServer *pServer = BLEDevice::createServer();
	BLEService *pService = pServer->createService(SERVICE_UUID);
	pCharacteristic = pService->createCharacteristic(
		CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	pService->start();
	BLEAdvertising *pAdvertising = pServer->getAdvertising();
	pAdvertising->addServiceUUID(pService->getUUID());
	pAdvertising->start();
}

int count = 0;

// this loop just publishes the value of count and then 
// increments it. You should see a stream of messages at 
// one second intervals.

void loop() {
    // Set values to send
  myData.trigger = !digitalRead(SWITCH_PIN);
  myData.pot = analogRead(POT_PIN);
  myData.battery = 4567;

	Serial.print(myData.trigger);
  Serial.print(",");
	Serial.print(myData.pot);
	Serial.print(",");
  Serial.print(myData.battery);
  Serial.println("");

	Serial.print("Sending: ");
	uint8_t buffer[80];
  memcpy(buffer, &myData, sizeof(myData));
	pCharacteristic->setValue(buffer, sizeof(myData));
	pCharacteristic->notify(); // send notification of change
	count++;

  // if (!myData.trigger) {
  //     esp_sleep_enable_ext0_wakeup(GPIO_NUM_0,0); //1 = High, 0 = Low
  //     Serial.println("Going to sleep now");
  //     delay(100);
  //     esp_deep_sleep_start();
  //}

  delay(100);
}