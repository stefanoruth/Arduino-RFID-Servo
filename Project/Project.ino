#include <RFID.h>
#include <SPI.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN,RST_PIN);

Servo servo1;
Servo servo2;

void setup() {
  //servo1.attach(9);  // Attach the servo on pin 9
  //servo2.attach(10); // Attach the servo on pin 10
}

void loop() {
  
}

// Move servo1 to a specefik position
void motor1(int pos) {
  servo1.write(pos);
  delay(5000); 
}

// Move servo2 to a specefik position
void motor2(int pos) {
  servo2.write(pos);
  delay(5000); 
}
