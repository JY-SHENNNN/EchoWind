#include <WiFi.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"
#include <Adafruit_PWMServoDriver.h>

#define SERVO_FREQ 50             // servo PWM renew frequency(HZ)
#define RETURN_PULSE 1900         // uplift to original point
#define USMIN 600                 // minimum pulse width
#define USMAX 2400                // max pulse width

//wifi info
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// MQTT Broker info
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;
const char* mqtt_user = SECRET_MQTTUSER;    
const char* mqtt_pass = SECRET_MQTTPASS; 
const char* mqtt_topic = "UCL/OPS/Garden/WST/dvp2/windSpeed_kph";

WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int count = 0;
bool messageChanged = false;
String lastMessage = "";

void callback(char* topic, byte* message, unsigned int length) {
  String currentMessage = "";
  for (int i = 0; i < length; i++) {
    currentMessage += (char)message[i];
  }

  if (currentMessage != lastMessage) {
    messageChanged = true;
    Serial.print("Message received from topic [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(currentMessage);
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void connectToMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32C6Client", mqtt_user, mqtt_pass)) {
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

void setup() {
  Serial.begin(115200);
  delay(1000);

  connectToWiFi();
  pwm.begin();
  pwm.setOscillatorFrequency(27000000); // calibration value
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
  for (int i = 0; i < 8; i++) {
    pwm.writeMicroseconds(i, RETURN_PULSE);
  }
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  randomSeed(analogRead(A0));
  
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  if (messageChanged) {
    messageChanged = false;
    Serial.println("change detected, trigger the wind chime");
  }
}
