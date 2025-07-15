#define button_pin 2
#define motor_pin 5

void setup() {
  pinMode(button_pin, INPUT_PULLUP);  // 按钮有上拉电阻
  pinMode(motor_pin, OUTPUT);
  randomSeed(analogRead(0));  // 初始化随机数种子
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(button_pin) == LOW) {
    int speed = random(0, 21);            // 生成 0-20 的随机数
    int pwmValue = map(speed, 0, 20, 0, 255);  // 映射到 0-255
    Serial.print("Speed = ");
    Serial.print(speed);
    Serial.print(" | PWM = ");
    Serial.println(pwmValue);

    analogWrite(motor_pin, 255);  // 启动电机
    Serial.print("trigger");
    delay(5000);  // 运行 5 秒

    analogWrite(motor_pin, 0);         // 停止电机
    Serial.println("Motor stopped.");
    delay(10000);  // 停止 10 秒
  }
}
