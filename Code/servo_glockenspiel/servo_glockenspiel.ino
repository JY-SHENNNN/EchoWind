#include <Servo.h>
Servo servo;

void setup() {
  servo.attach(5);
  servo.write(0); // initial point
  Serial.begin(9600);
}

void loop() {
  if (servo.read() != 0) {
    Serial.println("Not at origin!");
  }

  servo.write(90);
  Serial.println("Moving to 90°");
  delay(300);

  servo.write(0);
  Serial.println("Returning to 0°");
  //delay(5000);
}
