#include <Arduino.h>
#include "IPAddress.h"
#include "PubSubClient.h"
#include "gprs.h"

IPAddress server(127,0,0,1);
uint16_t port = 1883;

GPRSClient client = GPRSClient();
PubSubClient mqtt(server,port,client);

void reconnect();
void callback(char* topic, uint8_t* payload, unsigned int length);

void setup() {
  Serial.begin(9600);
  Serial.println("Start!");
  mqtt.setCallback(callback);
  if (client.begin(8,9,"internet")) {
    Serial.println("== Client Begin SUCCESS ==");
  }
  delay(100);
}

void loop() {
  if (!mqtt.loop())
    reconnect();
  delay(1000);
}

void callback(char* topic, uint8_t* payload, unsigned int length) {
  char buff[length+1];
  for (size_t i = 0; i < length; i++)
  {
    buff[i]=payload[i];
  }
  buff[length]='\0';
  mqtt.publish("in_topic/in",buff);
}

void reconnect(){
  if (!mqtt.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqtt.connect("ard")) {
      Serial.println("connected");
      delay(100);
      mqtt.subscribe("in_topic/out");
      delay(100);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}