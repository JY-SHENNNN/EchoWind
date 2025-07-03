#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVO_FREQ 50             // servo PWM update frequency
#define RETURN_PULSE 1900         // hang-up origin
#define RRTURN_ORI 1800
#define USMIN 600                 // servo smallest pulse width（0°）
#define USMAX 2400                // servo biggest pulse width（180°）

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int count = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Smart Glockenspiel with Random Wind Level");
  while(!Serial){
    delay(10);
  }
  pwm.begin();
  pwm.setOscillatorFrequency(27000000); // callibration
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

 pwm.writeMicroseconds(0, 1850);
 pwm.writeMicroseconds(1, 1850);
 pwm.writeMicroseconds(2, 1930);
 pwm.writeMicroseconds(3, 1930);
 pwm.writeMicroseconds(4, 1900);
 pwm.writeMicroseconds(5, 1900);
 pwm.writeMicroseconds(6, 1900);
 pwm.writeMicroseconds(7, 1900);

}

void loop() {
  pwm.writeMicroseconds(0, 1980);
  delay(40);  // click duration
  // rebounce
  pwm.writeMicroseconds(0, 1850);
  delay(500);  //

  pwm.writeMicroseconds(1, 1980);
  delay(40);  // click duration
  // rebounce
  pwm.writeMicroseconds(1, 1850);
  delay(500);  

  pwm.writeMicroseconds(2, 2150);
  delay(40);  // click duration
  // rebounce
  pwm.writeMicroseconds(2, 1930);
  delay(500);  

  pwm.writeMicroseconds(3, 2100);
  delay(40);  // click duration
  // rebounce
  pwm.writeMicroseconds(3, 1930);
  delay(500);  

  pwm.writeMicroseconds(4, 2200);
  delay(40);  // click duration
  // rebounce
  pwm.writeMicroseconds(4, 1900);
  delay(500);  

  pwm.writeMicroseconds(5, 2150);
  delay(40);  // click duration
  // rebounce
  pwm.writeMicroseconds(5, 1900);
  delay(500);  

  pwm.writeMicroseconds(6, 2100);
  delay(40);  // click duration
  // rebounce
  pwm.writeMicroseconds(6, 1900);
  delay(500); 

  pwm.writeMicroseconds(7, 2100);
  delay(40);  // click duration
  // rebounce
  pwm.writeMicroseconds(7, 1900);
  delay(500);  

}
