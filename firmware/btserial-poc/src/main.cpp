// note this is just proof of concept for ADC and BT serial (may not be used in final version)
// based on:
// https://randomnerdtutorials.com/esp32-bluetooth-classic-arduino-ide/
// https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/

//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 34;

// variable for storing the potentiometer value
int potValue = 0;

const int switchPin = 23;

int switchValue = 0;

void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  Serial.begin(9600);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
    // Reading potentiometer value
  potValue = analogRead(potPin);
  switchValue = !digitalRead(switchPin);
  Serial.print(switchValue);
  Serial.print(",");
  Serial.println(potValue);
  SerialBT.print(switchValue);
  SerialBT.print(",");
  SerialBT.println(potValue);
  delay(200);

  // if (Serial.available()) {
  //   SerialBT.write(Serial.read());
  // }
  // if (SerialBT.available()) {
  //   Serial.write(SerialBT.read());
  // }
  // delay(20);
}