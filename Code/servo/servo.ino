#define button_pin 2
#define motor_pin 5

bool lastButtonState = HIGH;         // 初始为未按下（INPUT_PULLUP）
bool currentButtonState;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(motor_pin, OUTPUT);
  randomSeed(analogRead(0));
  Serial.begin(9600);
}

void loop() {
  int reading = digitalRead(button_pin);

  // 检测按钮电平是否变化（可能抖动）
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // 重置防抖计时器
  }

  // 超过防抖延迟后再判断是否是真正的变化
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currentButtonState) {
      currentButtonState = reading;

      // 状态发生变化，触发一次动作
      int speed = random(0, 21);
      int pwmValue = map(speed, 0, 20, 0, 255);

      Serial.print("Speed = ");
      Serial.print(speed);
      Serial.print(" | PWM = ");
      Serial.println(pwmValue);

      analogWrite(motor_pin, 255);  // 实际应为 pwmValue，但你的代码是最大值
      Serial.println("Trigger");

      delay(5000);  // 电机运行5秒

      analogWrite(motor_pin, 0);   // 停止电机
      Serial.println("Motor stopped.");
    }
  }

  lastButtonState = reading;  // 更新上一轮读取值
}
