// analog pin A1
const int soundPin = A1;

void setup() {
  Serial.begin(9600);
  while (!Serial); 
  Serial.println("Sound Sensor v1.6 Initialized.");
}

void loop() {
  int soundValue = analogRead(soundPin);  // sound intensity（0~1023）
  Serial.print("Sound Value: ");
  Serial.println(soundValue);
  delay(100);  // update per 100 ms
}
