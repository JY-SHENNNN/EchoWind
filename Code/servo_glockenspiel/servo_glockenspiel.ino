#include <Servo.h>

Servo servoC;
Servo servoD;
Servo servoE;
Servo servoF;
Servo servoG;
Servo servoA;
Servo servoB;
Servo servoC1;

void setup() {
  Serial.begin(9600);
  servoC.attach(0);
  servoD.attach(1);
  servoE.attach(2);
  servoF.attach(3);
  servoG.attach(4);
  servoA.attach(5);  
  servoB.attach(6);  
  servoC1.attach(7);  

  // initial at 0
  servoC.write(0);
  servoD.write(0);
  servoE.write(0);
  servoF.write(0);
  servoG.write(0);
  servoA.write(0);
  servoB.write(0);
  servoC1.write(0);
  Serial.println("All servos set to 0°");
  delay(1000); 
}

void loop() {
  // down to click
  servoC.write(90);
  delay(100);
  servoD.write(90);
  delay(100);
  servoE.write(90);
  delay(100);
  servoF.write(90);
  delay(200);
  servoG.write(90);
  delay(200);
  servoA.write(90);
  delay(200);
  servoB.write(90);
  delay(100);
  servoC1.write(90);
  delay(100);
  Serial.println("All servos dropped to 90°");
  //delay(300);

  // up to 0
  servoC.write(0);
  servoD.write(0);
  servoE.write(0);
  servoF.write(0);
  servoG.write(0);
  servoA.write(0);
  servoB.write(0);
  servoC1.write(0);
  Serial.println("All servos returned to 0°");
  delay(3000);
}
