#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
  Serial.begin(9600);

  servo1.attach(5);  
  servo2.attach(7);  
  servo3.attach(8);  

  // initial at 0
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  Serial.println("All servos set to 0°");
  delay(1000); 
}

void loop() {
  // down to click
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  Serial.println("All servos dropped to 0°");
  delay(300);

  // up to 0
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  Serial.println("All servos returned to 90°");
  delay(3000);
}
