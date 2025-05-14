#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  for (int pos = 0; pos <= 180; pos++) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (int pos = 180; pos >= 0; pos--) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

