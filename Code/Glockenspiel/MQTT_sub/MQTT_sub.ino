#include <WiFi.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"
#include <Adafruit_PWMServoDriver.h>

#define SERVO_FREQ 50             // servo PWM renew frequency(HZ)
#define USMIN 600                 // minimum pulse width
#define USMAX 2400                // max pulse width
#define NUM_SERVOS 8
//wifi info
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// MQTT Broker info
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;
const char* mqtt_user = SECRET_MQTTUSER;    
const char* mqtt_pass = SECRET_MQTTPASS; 
//const char* mqtt_topic = "student/zczqjs2/windSpeed_kph";
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

int strikePositions[NUM_SERVOS] = {
  1980, 1980, 2150, 2100, 2200, 2150, 2100, 2100
};

// origin
int restPositions[NUM_SERVOS] = {
  1850, 1850, 1930, 1930, 1900, 1900, 1900, 1900
};

int BPW[NUM_SERVOS] = {
  4000, 3500, 3000, 2500, 2000, 1500, 1000, 500
};

int rest[NUM_SERVOS] = {
  500, 250, 125, 62, 40
}
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
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(messageHistory[i] * 3.6);
    }
    Serial.println("-------------------------");

    if (totalMessages == 8){
      totalMessages = 0;
      messageStored = true;
    }
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

int mapToBeaufort(float ms) { // map to tone by siggle or double
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

// int mapToBeaufort(float kph) {
//   if (kph < 1) return 0;
//   else if (kph <= 5) return 1;
//   else if (kph <= 11) return 2;
//   else if (kph <= 19) return 3;
//   else if (kph <= 28) return 4;
//   else if (kph <= 38) return 5;
//   else if (kph <= 49) return 6;
//   else return 7;  // 50+ kph
// }


void setup() {
  Serial.begin(115200);
  delay(1000);

  connectToWiFi();
  pwm.begin();
  pwm.setOscillatorFrequency(27000000); // calibration value
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
  // all servo back to origin
  for (int i = 0; i < NUM_SERVOS; i++) {
    pwm.writeMicroseconds(i, restPositions[i]);
  }

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void playWindChime(){
  for (int i=0; i<8; i++){
    windLevel[i] = mapToBeaufort(messageHistory[i] * 3.6);
  }

  for (int j=0; j<8; j++){
    if(windLevel[j] > 7){
      pwm.writeMicroseconds(windLevel[j] - 8, strikePositions[windLevel[j] - 8]);
      delay(rest[j]);  // duration of click
      pwm.writeMicroseconds(windLevel[j] - 8, restPositions[windLevel[j] - 8]);
      delay(500); // rebounce

      pwm.writeMicroseconds(windLevel[j] - 7, strikePositions[windLevel[j] - 7]);
      delay(rest[j]);  // duration of click
      pwm.writeMicroseconds(windLevel[j] - 7, restPositions[windLevel[j] - 7]);
      delay(500); // rebounce
    } else {
      pwm.writeMicroseconds(windLevel[j], strikePositions[windLevel[j]]);
      delay(BPW[j]);  // duration of click
      pwm.writeMicroseconds(windLevel[j], restPositions[windLevel[j]]);
      delay(500); // rebounce
    } 
  }
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  if (messageStored) {
    messageStored = false;
    Serial.println("8 value detected, trigger the wind chime");

    playWindChime();
    Serial.println("---- level map ----");
    for (int i = 0; i < 8; i++) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(windLevel[i]);
    }
    Serial.println("-------------------------");

  }
}
