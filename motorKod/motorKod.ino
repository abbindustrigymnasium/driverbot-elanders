#include <Servo.h>
#define motorPinRightDir  0//D2
#define motorPinRightSpeed 5//D1
 
Servo servo;
 
void setup() {
  // put your setup code here, to run once:
  servo.attach(D2);
  servo.write(0);
  delay(2000);

  // put your setup code here, to run once:
  pinMode(motorPinRightDir, OUTPUT);
  pinMode(motorPinRightSpeed, OUTPUT);

  Serial.begin(115200);

   // put your setup code here, to run once:
  pinMode(motorPinRightDir, OUTPUT);
  pinMode(motorPinRightSpeed, OUTPUT);

  Serial.begin(115200);
}
 
void loop() {
  // put your main code here, to run repeatedly:
 
  servo.write(0);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(180);
  delay(1000);

   int speed = 1024;
  int dir = 0;

  delay(2200);
  digitalWrite(motorPinRightDir, dir);
  analogWrite(motorPinRightSpeed, speed);
      delay(2200);      
  digitalWrite(motorPinRightDir, 1);
  analogWrite(motorPinRightSpeed, speed);

}
