//Kod som startar och connectar med MQTT samt innehåller motorkoden för de olika knapparna på websida

// Inkluderar bibliotek för WiFi, MQTT, Servo samt egna headerfiler för MQTT-anslutning och autentiseringsuppgifter.
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include "MQTTConnector.h"
#include "Credentials.h"

// Definierar pinnar för att styra riktning och hastighet på en motor.
#define motorPinRightDir  0 //D2
#define motorPinRightSpeed 5 //D1

Servo servo; // Skapar ett Servo-objekt.
WiFiClient wifiClient; // Skapar ett WiFiClient-objekt.
PubSubClient mqttClient(wifiClient); // Skapar ett PubSubClient-objekt för MQTT-kommunikation, använder wifiClient för nätverkskommunikation.

boolean mqttInitCompleted = false; // Flagga som indikerar om MQTT-initiering är klar.
String clientId = "IoTPractice-" + String(ESP.getChipId()); // Skapar ett unikt klient-ID baserat på ESP8266-chip-ID.

/* Callback-funktion för inkommande data */
void dataCallback(char* topic, byte* payload, unsigned int length)
{
  // Skapar en null-terminerad sträng från payload
  char payloadStr[length + 1];
  memset(payloadStr, 0, length + 1);
  strncpy(payloadStr, (char*)payload, length);

  // Skriver ut ämnet och payload till seriell monitor
  Serial.printf("Data    : dataCallback. Topic : [%s]\n", topic);
  Serial.printf("Data    : dataCallback. Payload : %s\n", payloadStr);

  // Kontrollera innehållet i payloadStr och utför åtgärder baserat på dess värde.
  if (strcmp(payloadStr, "Forward") == 0) {
    analogWrite(motorPinRightSpeed, 1024); // Sätt motorhastighet
    digitalWrite(motorPinRightDir, 1); // Sätt motorriktning framåt
    servo.write(90); // Sätt servoposition till 90 grader
  } else if (strcmp(payloadStr, "Backward") == 0) {
    analogWrite(motorPinRightSpeed, 1024); 
    digitalWrite(motorPinRightDir, 0); 
    servo.write(90);
  } else if (strcmp(payloadStr, "Left") == 0) {
    servo.write(0); 
  } else if (strcmp(payloadStr, "Right") == 0) {
    servo.write(180);
  } else if (strcmp(payloadStr, "Circle") == 0) {
    analogWrite(motorPinRightSpeed, 1024);
    digitalWrite(motorPinRightDir, 0);
    servo.write(0); 
    delay(10000);
    analogWrite(motorPinRightSpeed, 0); 
    servo.write(90); 
  } else if (strcmp(payloadStr, "stop") == 0) {
    analogWrite(motorPinRightSpeed, 0); 
    servo.write(90); 
  }

  // Cursed-seriens kommandon
  if (strcmp(payloadStr, "CursedForward") == 0) {
    analogWrite(motorPinRightSpeed, 1024); 
    digitalWrite(motorPinRightDir, 0); 
    servo.write(0); 
  } else if (strcmp(payloadStr, "CursedBackward") == 0) {
    analogWrite(motorPinRightSpeed, 1024);
    digitalWrite(motorPinRightDir, 1); 
    servo.write(180);
  } else if (strcmp(payloadStr, "CursedLeft") == 0) {
    analogWrite(motorPinRightSpeed, 1024); 
    digitalWrite(motorPinRightDir, 0); 
  } else if (strcmp(payloadStr, "CursedRight") == 0) {
    analogWrite(motorPinRightSpeed, 1024); 
    digitalWrite(motorPinRightDir, 1);
  } else if (strcmp(payloadStr, "CursedCircle") == 0) {
    int speed = 1024; 
    digitalWrite(motorPinRightDir, 0); 
    analogWrite(motorPinRightSpeed, speed); 
    delay(2200); 
    digitalWrite(motorPinRightDir, 1); 
    analogWrite(motorPinRightSpeed, speed); 
    delay(2200); 
    digitalWrite(motorPinRightDir, 0); 
    analogWrite(motorPinRightSpeed, speed); 
    delay(2200); // 
    digitalWrite(motorPinRightDir, 1);
    analogWrite(motorPinRightSpeed, speed);
    delay(2200); 
    analogWrite(motorPinRightSpeed, 0); 
  } else if (strcmp(payloadStr, "stop") == 0) {
    analogWrite(motorPinRightSpeed, 0); 
    servo.write(90); 
  }
}

// Funktion för att hantera anslutning till MQTT-servern
void performConnect()
{
  uint16_t connectionDelay = 5000; // Fördröjning mellan anslutningsförsök
  while (!mqttClient.connected())
  {
    Serial.printf("Trace   : Attempting MQTT connection...\n");
    if (mqttClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY))
    {
      Serial.printf("Trace   : Connected to Broker.\n");

      // Prenumererar på ditt ämne efter att anslutningen lyckats.
      MQTTSubscribe(TOPIC);
    }
    else
    {
      Serial.printf("Error!  : MQTT Connect failed, rc = %d\n", mqttClient.state());
      Serial.printf("Trace   : Trying again in %d msec.\n", connectionDelay);
      delay(connectionDelay); // Vänta innan ett nytt anslutningsförsök
    }
  }
}

// Funktion för att publicera meddelanden på ett MQTT-ämne
boolean MQTTPublish(const char* topic, char* payload)
{
  boolean retval = false;
  if (mqttClient.connected())
  {
    retval = mqttClient.publish(topic, payload); // Publicerar meddelandet
  }
  return retval;
}

// Funktion för att prenumerera på ett MQTT-ämne
boolean MQTTSubscribe(const char* topicToSubscribe)
{
  boolean retval = false;
  if (mqttClient.connected())
  {
    retval = mqttClient.subscribe(topicToSubscribe); // Prenumererar på ämnet
  }
  return retval;
}

// Funktion som kontrollerar om MQTT är anslutet
boolean MQTTIsConnected()
{
  return mqttClient.connected();
}

// Initialiserar MQTT-anslutningen
void MQTTBegin()
{
  mqttClient.setServer(MQTT_BROKER, MQTT_BROKER_PORT); // Sätter MQTT-server och port
  mqttClient.setCallback(dataCallback); // Sätter callback-funktion för inkommande meddelanden
  mqttInitCompleted = true; 
}

// Hanterar MQTT-loop
void MQTTLoop()
{
  if (mqttInitCompleted)
  {
    if (!MQTTIsConnected())
    {
      performConnect(); // Försöker ansluta om inte ansluten
    }
    mqttClient.loop(); // Hanterar inkommande och utgående MQTT-meddelanden
  }
}

// Setup-funktion som körs en gång vid uppstart
void setup()
{
  Serial.begin(115200); // Startar seriell kommunikation med 115200 baud
  WiFi.begin(STA_SSID, STA_PASS); // Startar WiFi-anslutning
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); // Bekräftar WiFi-anslutning

  pinMode(motorPinRightDir, OUTPUT); // Sätter riktning för motorpinnen som utgång
  pinMode(motorPinRightSpeed, OUTPUT); // Sätter hastighet för motorpinnen som utgång
  MQTTBegin(); // Initierar MQTT

  servo.attach(D2); // Fäster servot till pin D2
  servo.write(0);   // Initierar servoposition till 0 grader
}

// Loop-funktion som körs kontinuerligt
void loop()
{
  MQTTLoop(); // Kör MQTT-loop
}

