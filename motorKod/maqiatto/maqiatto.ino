#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include "MQTTConnector.h"
#include "Credentials.h"

#define motorPinRightDir  0 //D2
#define motorPinRightSpeed 5 //D1

Servo servo;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

boolean mqttInitCompleted = false;
String clientId = "IoTPractice-" + String(ESP.getChipId());

/* Incoming data callback. */
void dataCallback(char* topic, byte* payload, unsigned int length)
{
  // Create a null-terminated string from the payload
  char payloadStr[length + 1];
  memset(payloadStr, 0, length + 1);
  strncpy(payloadStr, (char*)payload, length);

  Serial.printf("Data    : dataCallback. Topic : [%s]\n", topic);
  Serial.printf("Data    : dataCallback. Payload : %s\n", payloadStr);

  // Check if the payload is "hej"
  if (strcmp(payloadStr, "Forward") == 0) {
   analogWrite(motorPinRightSpeed, 1024);
   digitalWrite(motorPinRightDir, 1);
  } else if (strcmp(payloadStr, "Backward") == 0) {
   analogWrite(motorPinRightSpeed, 1024);
   digitalWrite(motorPinRightDir, 0);
  } else if (strcmp(payloadStr, "Left") == 0) {
    servo.write(0);
  } else if (strcmp(payloadStr, "Right") == 0) {
    servo.write(180);
  } else if (strcmp(payloadStr, "stop") == 0) {
    analogWrite(motorPinRightSpeed, 0);
    servo.write(90);
 
}
}
void performConnect()
{
  uint16_t connectionDelay = 5000;
  while (!mqttClient.connected())
  {
    Serial.printf("Trace   : Attempting MQTT connection...\n");
    if (mqttClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY))
    {
      Serial.printf("Trace   : Connected to Broker.\n");

      /* Subscription to your topic after connection was succeeded.*/
      MQTTSubscribe(TOPIC);
    }
    else
    {
      Serial.printf("Error!  : MQTT Connect failed, rc = %d\n", mqttClient.state());
      Serial.printf("Trace   : Trying again in %d msec.\n", connectionDelay);
      delay(connectionDelay);
    }
  }
}

boolean MQTTPublish(const char* topic, char* payload)
{
  boolean retval = false;
  if (mqttClient.connected())
  {
    retval = mqttClient.publish(topic, payload);
  }
  return retval;
}

boolean MQTTSubscribe(const char* topicToSubscribe)
{
  boolean retval = false;
  if (mqttClient.connected())
  {
    retval = mqttClient.subscribe(topicToSubscribe);
  }
  return retval;
}

boolean MQTTIsConnected()
{
  return mqttClient.connected();
}

void MQTTBegin()
{
  mqttClient.setServer(MQTT_BROKER, MQTT_BROKER_PORT);
  mqttClient.setCallback(dataCallback);
  mqttInitCompleted = true;
}

void MQTTLoop()
{
  if (mqttInitCompleted)
  {
    if (!MQTTIsConnected())
    {
      performConnect();
    }
    mqttClient.loop();
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(STA_SSID, STA_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  pinMode(motorPinRightDir, OUTPUT);
  pinMode(motorPinRightSpeed, OUTPUT);
  MQTTBegin();

  servo.attach(D2); // Attach servo to pin D1
  servo.write(0);   // Initialize servo position to 0 degrees
}

void loop()
{
  MQTTLoop();
}
