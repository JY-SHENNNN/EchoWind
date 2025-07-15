#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVO_FREQ 50            // servo PWM update frequency (HZ)
#define USMIN 600                 // servo smallest pulse width（0°）
#define USMAX 2400                // servo biggest pulse width（180°）
#define NUM_SERVOS 8
#define button_pin 5
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int count = 0;
int windLevel[8]; //used to store newest Beaufort level
int strikePositions[NUM_SERVOS] = {
  1980, 1980, 2150, 2100, 2200, 2150, 2100, 2100
};

// origin
int restPositions[NUM_SERVOS] = {
  1850, 1850, 1930, 1900, 1900, 1900, 1900, 1900
};

void setup() {
  Serial.begin(9600);
  Serial.println("Smart Glockenspiel with Random Wind Level");
  while(!Serial){
    delay(10);
  }
  pinMode(button_pin, INPUT_PULLUP);  // new added pull up resister
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
  int windBeaufortLevel = random(0, 13);  // random generate
  windLevel[count] = windBeaufortLevel;
  count++;
  if (count == 8){
    for(int j=0; j<8; j++){
      if(digitalRead(button_pin) == LOW){
        if (windLevel[j] <= 7){
          pwm.writeMicroseconds(windLevel[j], strikePositions[windLevel[j]]);
          delay(40);  // duration of click
          pwm.writeMicroseconds(windLevel[j], restPositions[windLevel[j]]);
          delay(1000); // rebounce
        } else{
          pwm.writeMicroseconds(windLevel[j] - 8, strikePositions[windLevel[j] - 8]);
          delay(40);  // duration of click
          pwm.writeMicroseconds(windLevel[j] - 8, restPositions[windLevel[j] - 8]);
          delay(500); // rebounce

          pwm.writeMicroseconds(windLevel[j] - 7, strikePositions[windLevel[j] - 7]);
          delay(40);  // duration of click
          pwm.writeMicroseconds(windLevel[j] - 7, restPositions[windLevel[j] - 7]);
          delay(500); // rebounce
        }
      }
    }

  Serial.println("Complete. Resting for 80 seconds...");
  delay(80000);  // 80,000 ms = 80 seconds

  count = 0; 
  } 
}
