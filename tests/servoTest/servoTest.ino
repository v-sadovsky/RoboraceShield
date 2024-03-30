#include <Servo.h>

#define SRV_PIN 9

Servo srv;

void setup() {
  srv.attach(SRV_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:
  srv.write(0);
  delay(1000);
  srv.write(90);
  delay(1000);
  srv.write(180);
  delay(3000);

}
