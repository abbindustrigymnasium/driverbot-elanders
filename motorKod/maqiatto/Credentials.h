#ifndef ARDUINO_CREDENTIALS_H
#define ARDUINO_CREDENTIALS_H

/* WiFi Credentials to connect Internet */
#define STA_SSID "ABB_Gym_IOT"
#define STA_PASS "Welcome2abb"

/* Provide MQTT broker credentials as denoted in maqiatto.com. */
#define MQTT_BROKER       "maqiatto.com"
#define MQTT_BROKER_PORT  1883
#define MQTT_USERNAME     "elvirasofiaandersson@hotmail.com"
#define MQTT_KEY          "ElSoAn7823"


/* Provide topic as it is denoted in your topic list. 
 * For example mine is : cadominna@gmail.com/topic1
 * To add topics, see https://www.maqiatto.com/configure
 */
#define TOPIC    "elvirasofiaandersson@hotmail.com/lampa"

#endif /* ARDUINO_CREDENTIALS_H */
