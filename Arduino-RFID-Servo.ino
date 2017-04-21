#include <Servo.h>

Servo servo1;
Servo servo2:

void setup() {
  servo1.attach(9);  // attaches the servo on pin 9 to the servo object
  servo2.attach(10);
}

void loop() {
  //servoPos(0);
  //servoPos(180);
}

void motor1(int pos) {
  servo1.write(pos);
  delay(5000); 
}

void motor2(int pos) {
  servo2.write(pos);
  delay(5000); 
}
