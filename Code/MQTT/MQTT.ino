#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

#define motor_pin 5

// Replace with your own WiFi credentials
const char* ssid          = SECRET_SSID;
const char* password      = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;

const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;
const char* mqtt_topic = "UCL/OPS/Garden/WST/dvp2/windSpeed_kph";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

String lastMessage = "";
float speed_kph;
bool messageChanged = false;
bool enableMotor = false;

// Function to reconnect to the MQTT broker
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

// Callback function for received MQTT messages
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


// void driveServo() {
//   if (enableMotor){
//     enableMotor = false;
//     int approxInt = round(speed_kph);
//     if (approxInt <= 8){
//       digitalWrite(motor_pin, HIGH);
//       delay(500);
//       digitalWrite(motor_pin, LOW);
//       delay(5000);
//     } else if (approxInt > 8 && approxInt <= 9) {
//       digitalWrite(motor_pin, HIGH);
//       delay(1000);
//       digitalWrite(motor_pin, LOW);
//       delay(5000);
//     } else if (approxInt > 9 && approxInt <= 10) {
//       digitalWrite(motor_pin, HIGH);
//       delay(3000);
//       digitalWrite(motor_pin, LOW);
//       delay(5000);
//     } else if (approxInt > 10){
//       digitalWrite(motor_pin, HIGH);
//       delay(5000);
//       digitalWrite(motor_pin, LOW);
//       delay(5000);
//     }
//   }
// }

// void PWMalter() { //linear changes are hard to see the boundary
//   int kph = 16;
//   //int pwmValue = map(speed_kph, 0, 20, 0, 150); // linear reflect speed to PWM
//   analogWrite(motor_pin, pwmValue); 
//   Serial.println("pwmvalue");
//   Serial.print(pwmValue);
//   delay(5000); 
// }

void PWMWave() {
  int pwmValue = getPwmValue(speed_kph);
  analogWrite(motor_pin, pwmValue); 
  Serial.println(pwmValue);
  delay(1000);
  analogWrite(motor_pin, LOW);
  delay(2000);
  // analogWrite(motor_pin, 30); 
  // Serial.println(30);
  // //Serial.print(pwmValue);
  // delay(1000);
  // analogWrite(motor_pin, LOW);
  // delay(2000);
  // analogWrite(motor_pin, LOW);
  // delay(2000);
  // analogWrite(motor_pin, 80); 
  // Serial.println(80);
  // //Serial.print(pwmValue);
  // delay(1000);
  // analogWrite(motor_pin, LOW);
  // delay(2000);
  // analogWrite(motor_pin, 120); 
  // Serial.println(120);
  // //Serial.print(pwmValue);
  // delay(1000);
  // analogWrite(motor_pin, LOW);
  // delay(2000);
  // analogWrite(motor_pin, 180); 
  // Serial.println(180);
  // //Serial.print(pwmValue);
  // delay(1000);
  // analogWrite(motor_pin, LOW);
  // delay(2000);
  // analogWrite(motor_pin, 255); 
  // Serial.println(255);
  // //Serial.print(pwmValue);
  // delay(1000);
  // analogWrite(motor_pin, LOW);
  // delay(2000);

}


int getPwmValue(float kph) {
  if (kph < 1) return map(kph, 0, 1, 0, 10);    // 0-10 KPH → PWM 30-80 weak
  else if (kph < 5) return map(kph, 1, 5, 10, 30); // 10-30 → PWM 80-180 
  else if (kph < 11) return map(kph, 5, 11, 30, 80);
  else if (kph < 19) return map(kph, 11, 19, 80, 120);
  else if (kph < 28) return map(kph, 19, 28, 120, 180);
  else return map(kph, 28, 50, 180, 255);         // 30-50 → PWM 180-255 strong
}


void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(motor_pin, OUTPUT);
  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set MQTT server and callback function
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();  // Process incoming MQTT messages

  if (messageChanged) {
    messageChanged = false;
    Serial.println("change detected, trigger the wind chime");
    enableMotor = true;
    //driveServo();
    //PWMalter();
    PWMWave();
  }
}
