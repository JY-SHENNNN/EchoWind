#include <Servo.h>  
Servo myservo;      
const int servoPin = 5; 

void setup() {
  myservo.attach(servoPin); 
}

void loop() {
  myservo.write(0);     // rotate to 0 degree
  delay(100);          
  myservo.write(90);    // rotate to 90 degree
  delay(1000);
  Serial.println(90);
  myservo.write(0);   // rotate to 0 degree
  delay(100);

}