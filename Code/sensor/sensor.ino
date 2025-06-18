#include <WiFiNINA.h>
#include <PubSubClient.h>
//#include "arduino_secrets.h"    // Include sensitive data from arduino_secrets.h

#define windSensorPin 8  // First touch sensor connected to 8
int pulseCount = 0; // count generated pulse number
unsigned long lastTime = 0;
const unsigned long sampleInterval = 3000; //sampleing time interval


void countPulse() {
  pulseCount++;
}


void setup() {
  Serial.begin(115200);  // Start the serial monitor to show output
  pinMode(windSensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(windSensorPin), countPulse, FALLING); // NPN 
  lastTime = millis();
}

void loop() {
 unsigned long currentTime = millis();
 float durationInSeconds = sampleInterval/1000; //time in seconds
  if (currentTime - lastTime >= sampleInterval) {
    detachInterrupt(digitalPinToInterrupt(windSensorPin));

    float windSpeed = (pulseCount / durationInSeconds) * 1.75;


    Serial.print("Pulses: ");
    Serial.print(pulseCount);
    Serial.print(" -> Wind Speed: ");
    Serial.print(windSpeed);
    Serial.println(" km/h");

    pulseCount = 0;
    lastTime = currentTime;

    attachInterrupt(digitalPinToInterrupt(windSensorPin), countPulse, FALLING);
  }
  
}
