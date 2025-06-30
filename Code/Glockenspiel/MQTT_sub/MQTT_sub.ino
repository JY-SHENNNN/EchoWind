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
bool messageStored = false;
String lastMessage = "";
float messageHistory[8]; //used to store newest 8 message
int windLevel[8]; //used to store newest Beaufort level
int messageIndex = 0;
int totalMessages = 0;

void callback(char* topic, byte* message, unsigned int length) {
  String currentMessage = "";
  for (int i = 0; i < length; i++) {
    currentMessage += (char)message[i];
  }

  if (currentMessage != lastMessage) {
    messageChanged = true;
    lastMessage = currentMessage;
    // store the value

    
    messageHistory[messageIndex] = currentMessage.toFloat();
    messageIndex = (messageIndex + 1) % 8;
    if (totalMessages < 8) totalMessages++;

    Serial.println("---- Last 8 Messages ----");
    for (int i = 0; i < totalMessages; i++) {
      int idx = (messageIndex + i) % 8;  // 从最旧开始打印
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(messageHistory[idx]);
    }
    Serial.println("-------------------------");

    if (totalMessages == 8){
      totalMessages = 0;
      messageStored = true;
    }
    // Serial.print("Message received from topic [");
    // Serial.print(topic);
    // Serial.print("]: ");
    // Serial.println(currentMessage);
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

int mapToBeaufort(float kph) {
  if (kph < 1) return 0;
  else if (kph <= 5) return 1;
  else if (kph <= 11) return 2;
  else if (kph <= 19) return 3;
  else if (kph <= 28) return 4;
  else if (kph <= 38) return 5;
  else if (kph <= 49) return 6;
  else return 7;  // 50+ kph
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
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  if (messageStored) {
    messageStored = false;
    Serial.println("8 value detected, trigger the wind chime");

    for (int i=0; i<8; i++){
      windLevel[i] = mapToBeaufort(messageHistory[i]);
    }
    for (int j=7; j>0; j--){
      pwm.writeMicroseconds(windLevel[j], 2400);
      delay(80);
      pwm.writeMicroseconds(windLevel[j], RETURN_PULSE);
      delay(300);
    }
    // //convert string message to float
    // float windSpeed = lastMessage.toFloat();
    // Serial.print("wind speed (kph) :");
    // Serial.println(windSpeed);

    //convert windspeed to Beaufort 0-7 scale
    // int windLevel[8] = mapToBeaufort(windSpeed);
    // Serial.print("Beaufort level: ");
    // Serial.println(windLevel);

    // pwm.writeMicroseconds(windLevel, 2400);
    // delay(80);
    // pwm.writeMicroseconds(windLevel, RETURN_PULSE);
    // delay(300);
  }
}
