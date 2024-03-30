
#define EN     4
#define DC_PWM 5
#define IN2    6
#define IN1    7

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN, OUTPUT);

  digitalWrite(EN, 1);
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);

  analogWrite(DC_PWM, 130);

}

void loop() {
  delay(250);
}