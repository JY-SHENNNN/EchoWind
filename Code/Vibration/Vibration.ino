#define motor_pin 1
void setup() {
  // put your setup code here, to run once:
  pinMode(motor_pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(motor_pin, HIGH);
  delay(1000);
  digitalWrite(motor_pin, LOW);
  delay(5000);
  digitalWrite(motor_pin, HIGH);
  delay(3000);
  digitalWrite(motor_pin, LOW);
  delay(5000);
  digitalWrite(motor_pin, HIGH);
  delay(5000);
  digitalWrite(motor_pin, LOW);
  delay(5000);
}
