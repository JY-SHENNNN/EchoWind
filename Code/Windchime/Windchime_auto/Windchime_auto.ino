#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

#define motor_pin 5

const char* ssid          = SECRET_SSID;
const char* password      = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;

const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;
//const char* mqtt_topic = "UCL/OPS/Garden/WST/dvp2/windSpeed_kph";
const char* mqtt_topic = "student/zczqjs2/windSpeed_kph";
WiFiClient wifiClient;
PubSubClient client(wifiClient);

String lastMessage = "";
float speed_kph;
bool messageChanged = false;
bool enableMotor = false;


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

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting to connect to MQTT broker...");
    if (client.connect("MKR1010_Client", mqtt_username, mqtt_password)) {
      Serial.println("Connected successfully!");
      if (client.subscribe(mqtt_topic)) {
        Serial.println("Successfully subscribed to topic!");
      } else {
        Serial.println("Failed to subscribe!");
      }
    } else {
      Serial.print("Connection failed, error code: ");
      Serial.print(client.state());
      Serial.println(". Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String currentMessage = "";
  for (int i = 0; i < length; i++) {
    currentMessage += (char)payload[i];
  }

  if (currentMessage != lastMessage) {
    messageChanged = true;
    Serial.print("Message received from topic [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(currentMessage);
  }

  speed_kph = currentMessage.toFloat();
  lastMessage = currentMessage;
}

void PWMWave() {
  int pwmValue = getPwmValue(speed_kph * 3.6 );
  analogWrite(motor_pin, pwmValue);
  Serial.println(pwmValue);
  delay(5000);
  analogWrite(motor_pin, LOW);
  delay(500);
}

int getPwmValue(float ms) {
  if (ms < 3.3) return map(ms, 0, 3.3, 0, 51);
  else if (ms < 10.7) return map(ms, 3.3, 10.7, 51, 102);
  else if (ms < 20.7) return map(ms, 10.7, 20.7, 102, 153);
  else if (ms < 32.6) return map(ms, 20.7, 32.6, 153, 204);
  else return map(ms, 32.6, 50, 204, 255);
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(motor_pin, OUTPUT);
  connectToWiFiWithTimeout();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFiWithTimeout();
    delay(1000);
  }
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  PWMWave();  //anable motor
}

