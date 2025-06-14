#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVO_FREQ 50             // 舵机 PWM 更新频率（Hz）
#define RETURN_PULSE 1900         // 抬起位置（“原点”）
#define USMIN 600                 // 舵机最小脉宽（0°）
#define USMAX 2400                // 舵机最大脉宽（180°）

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


void setup() {
  Serial.begin(9600);
  Serial.println("Smart Glockenspiel with Random Wind Level");
  while(!Serial){
    delay(10);
  }
  pwm.begin();
  pwm.setOscillatorFrequency(27000000); // 推荐校准值
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  // 所有舵机初始化回到“原点”
  for (int i = 0; i < 8; i++) {
    pwm.writeMicroseconds(i, RETURN_PULSE);
  }

  // 初始化随机数种子
  randomSeed(analogRead(A0));
}

void loop() {
  // 随机生成一个 Beaufort 风速等级（0 到 7）
  int windBeaufortLevel = random(0, 8);          // 代表风级
  int servoChannel = windBeaufortLevel;          // 用风级编号选通道
  //int strikePulse = beaufortToPulse(windBeaufortLevel);

  Serial.print("Wind Level: "); Serial.println(windBeaufortLevel);
  Serial.print(" → Servo "); Serial.println(servoChannel);
  //Serial.print(" | Strike Pulse: "); Serial.println(strikePulse);

  // 敲击动作
  pwm.writeMicroseconds(servoChannel, 2400);
  delay(80);  // 敲击保持时间

  // 回弹抬起
  pwm.writeMicroseconds(servoChannel, RETURN_PULSE);
  delay(300);  // 2 秒间隔，再下一次风来
}
