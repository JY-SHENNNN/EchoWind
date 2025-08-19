#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

// ====== WiFi & MQTT Settings ======
const char* ssid          = SECRET_SSID;
const char* password      = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;

const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;
const char* mqtt_topic = "student/zczqjs2/windSpeed_kph";

WiFiClient wifiClient;
PubSubClient client(wifiClient);
extern "C" char* dtostrf(double val, signed char width, unsigned char prec, char* s);

// ====== Wind Sensor ======
#define windSensorPin 7
volatile int pulseCount = 0;
unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 3000; // 3s sampling
float lastWindSpeed = 0;
float windSpeed = 0;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000; // min 10s between sends
const float threshold = 1.0; // Min 1 km/h difference to trigger message

// average for 3 times sampleing
float samples[3] = {0, 0, 0};
int sampleIndex = 0;
int samplesFilled = 0;

void countPulse() {
  pulseCount++;
}

// ====== Setup Functions ======
void setup() {
  Serial.begin(115200);
  pinMode(windSensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(windSensorPin), countPulse, FALLING);

  connectToWiFiWithTimeout();
  Serial.println("\nWiFi connected");

  client.setServer(mqtt_server, mqtt_port);
  connectToMQTT();

  lastSampleTime = millis();
  lastSendTime = millis();
}

// ====== Main Loop ======
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFiWithTimeout();
    delay(1000);
  }
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  unsigned long currentTime = millis();
  if (currentTime - lastSampleTime >= sampleInterval) {
    detachInterrupt(digitalPinToInterrupt(windSensorPin));
    float durationSec = sampleInterval / 1000.0;
    windSpeed = ((pulseCount / durationSec) * 1.75) / 3.6 ; // kph

    Serial.print("Pulses: ");
    Serial.print(pulseCount);
    Serial.print(" -> Wind Speed: ");
    Serial.print(windSpeed);
    Serial.println(" km/h");

   
    samples[sampleIndex] = windSpeed;
    sampleIndex = (sampleIndex + 1) % 3;
    if (samplesFilled < 3) samplesFilled++;

    // float delta = abs(windSpeed - lastWindSpeed);
    // if (delta >= threshold && currentTime - lastSendTime >= sendInterval) {
    if (currentTime - lastSendTime >= sendInterval && samplesFilled == 3) {
      
      float avg = (samples[0] + samples[1] + samples[2]) / 3.0;
      Serial.print("Average (3 samples): ");
      Serial.print(avg);
      Serial.println(" km/h");
      sendMQTT(avg);
      lastSendTime = currentTime;
      lastWindSpeed = avg;
    }

    pulseCount = 0;
    lastSampleTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(windSensorPin), countPulse, FALLING);
  }
}

// ====== MQTT Connect & Send ======
void connectToMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("WindSensorClient", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

void sendMQTT(float windSpeed) {
  char mqtt_message[64]; 
  sprintf(mqtt_message, "%.2f", windSpeed);
  Serial.print("Sending MQTT message: ");
  Serial.println(mqtt_message);
  if (client.publish(mqtt_topic, mqtt_message)) {
    Serial.println("Published successfully");
  } else {
    Serial.println("Failed to publish");
  }
}

void connectToWiFiWithTimeout() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed. Restarting...");
    NVIC_SystemReset();  // restart to connect
  }
}
