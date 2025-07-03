#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVO_FREQ 50            // servo PWM update frequency (HZ)
#define USMIN 600                 // servo smallest pulse width（0°）
#define USMAX 2400                // servo biggest pulse width（180°）
#define NUM_SERVOS 8

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int count = 0;
int strikePositions[NUM_SERVOS] = {
  1980, 1980, 2150, 2100, 2200, 2150, 2100, 2100
};

// origin
int restPositions[NUM_SERVOS] = {
  1850, 1850, 1930, 1930, 1900, 1900, 1900, 1900
};

void setup() {
  Serial.begin(9600);
  Serial.println("Smart Glockenspiel with Random Wind Level");
  while(!Serial){
    delay(10);
  }
  pwm.begin();
  pwm.setOscillatorFrequency(27000000); // calibration
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  // all servo back to origin
  for (int i = 0; i < NUM_SERVOS; i++) {
    pwm.writeMicroseconds(i, restPositions[i]);
  }

  // seed of random 
  randomSeed(analogRead(A0));
}

void loop() {
  // random generate Beaufort wind level (0-7)
  int windBeaufortLevel = random(0, 8);          // wind level
  count++;
  if (count == 8){
    delay(3000);
    count = 0;
  }
  int servoChannel = windBeaufortLevel;          // wind level represent servo number
  //int strikePulse = beaufortToPulse(windBeaufortLevel);

  Serial.print("Wind Level: "); Serial.println(windBeaufortLevel);
  Serial.print(" → Servo "); Serial.println(servoChannel);
  //Serial.print(" | Strike Pulse: "); Serial.println(strikePulse);

  pwm.writeMicroseconds(servoChannel, strikePositions[servoChannel]);
  delay(40);  // duration of click
  pwm.writeMicroseconds(servoChannel, restPositions[servoChannel]);
  delay(500); // rebounce
  
}
