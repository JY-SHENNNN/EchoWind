#include <WiFi.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"
#include <Adafruit_PWMServoDriver.h>
#include "esp_system.h"

#define SERVO_FREQ 50
#define NUM_SERVOS 8

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;
const char* mqtt_user = SECRET_MQTTUSER;
const char* mqtt_pass = SECRET_MQTTPASS;
const char* mqtt_topic = "student/zczqjs2/windSpeed_kph";

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

String lastMessage = "";

int strikePositions[NUM_SERVOS] = {2100, 2100, 2150, 2200, 2100, 2150, 1980, 1980};
int restPositions[NUM_SERVOS]   = {1930, 1900, 1900, 1900, 1920, 1930, 1900, 1870};

// === Beaufort mapping ===
int mapToBeaufort(float ms) {
  if (ms < 0.2) return 0;
  else if (ms <= 1.5) return 1;
  else if (ms <= 3.3) return 2;
  else if (ms <= 5.4) return 3;
  else if (ms <= 7.9) return 4;
  else if (ms <= 10.7) return 5;
  else if (ms <= 13.8) return 6;
  else if (ms <= 17.1) return 7;
  else if (ms <= 20.7) return 8;
  else if (ms <= 24.4) return 9;
  else if (ms <= 28.4) return 10;
  else if (ms <= 32.6) return 11;
  else return 12;
}

void playSingleBeaufort(int level) {
  if (level > 7) {
    pwm.writeMicroseconds(level - 8, strikePositions[level - 8]);
    delay(40);
    pwm.writeMicroseconds(level - 8, restPositions[level - 8]);
    delay(500);

    pwm.writeMicroseconds(level - 7, strikePositions[level - 7]);
    delay(40);
    pwm.writeMicroseconds(level - 7, restPositions[level - 7]);
    delay(1000);
  } else {
    pwm.writeMicroseconds(level, strikePositions[level]);
    delay(40);
    pwm.writeMicroseconds(level, restPositions[level]);
    delay(1000);
  }
}

void connectToWiFiWithTimeout() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 15000) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi failed. Restarting ESP32...");
    esp_restart();
  }
}

void connectToMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// === MQTT callback: get message then play ===
void callback(char* topic, byte* message, unsigned int length) {
  String currentMessage = "";
  for (unsigned int i = 0; i < length; i++) {
    currentMessage += (char)message[i];
  }
  lastMessage = currentMessage;

  float val = currentMessage.toFloat();
  if (!isnan(val)) {
    float ms = val / 3.6f; // kph -> m/s
    int bft = mapToBeaufort(ms);

    Serial.print("RX: "); Serial.print(val); Serial.print(" kph -> ");
    Serial.print(ms, 2); Serial.print(" m/s -> Beaufort ");
    Serial.println(bft);

    playSingleBeaufort(bft); 
  } else {
    Serial.print("Non-numeric payload: ");
    Serial.println(currentMessage);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  connectToWiFiWithTimeout();

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
  for (int i = 0; i < NUM_SERVOS; i++) {
    pwm.writeMicroseconds(i, restPositions[i]);
  }

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  connectToMQTT();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFiWithTimeout();
    delay(1000);
  }
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();
}
