#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int speed = 0;  // speed value will be randomly generated

void setup() {
  myservo.attach(2);  // attach the servo on pin 2
  randomSeed(analogRead(0));  // initialize random number generator
}

void loop() {
  speed = random(0, 21);  // generate a random speed between 0 and 20
  Serial.print("Speed = ");
  Serial.println(speed);

  if (speed <= 10) {
    for (int pos = 0; pos <= 30; pos++) {
      myservo.write(pos);
      delay(15);  // small delay between steps for smooth motion
    }
    for (int pos = 30; pos >= 0; pos--) {
      myservo.write(pos);
      delay(15);
    }
  } else if (speed > 10) {
    for (int pos = 0; pos <= 180; pos++) {
      myservo.write(pos);
      delay(10);
    }
    for (int pos = 180; pos >= 0; pos--) {
      myservo.write(pos);
      delay(10);
    }
  }

  delay(5000);  // wait for 20 seconds before next cycle
}
