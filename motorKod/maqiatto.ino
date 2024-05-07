#include <ESP8266WiFi.h>
#include <Arduino.h>

#include "MQTTConnector.h"
#include "Credentials.h"

#include <Servo.h>

#define WIFI_TIMEOUT 10000
#define LOOP_TIMEOUT 10000

#define motorPinRightDir  0//D2
#define motorPinRightSpeed 5//D1
 
Servo servo;

void WiFiBegin(const char* ssid, const char* pass)
{
  WiFi.begin(ssid, pass);
  Serial.printf("Waiting for AP connection ...\n");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(WIFI_TIMEOUT);
    Serial.printf(".");
  }
  IPAddress ip = WiFi.localIP();
  Serial.printf("\nConnected to AP. IP : %d.%d.%d.%d\n", 
    ip[0],ip[1],ip[2],ip[3]);
}

void setup() 
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  WiFiBegin(STA_SSID, STA_PASS);
  MQTTBegin();

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

void loop() 
{
  MQTTLoop();
  if(MQTTPublish(TOPIC, "a"))
  {
    Serial.printf("MQTTPublish was succeeded.\n");
  }
  delay(LOOP_TIMEOUT);

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
