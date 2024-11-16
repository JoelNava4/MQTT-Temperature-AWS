#include "SensorDHT.h"
#include <Arduino.h>

SensorDHT::SensorDHT(int pin, int type, MQTTHandler* mqttHandler, const char* UPDATE_TOPIC)
    : dhtPin(pin), dht(pin, type), mqttHandler(mqttHandler), UPDATE_TOPIC(UPDATE_TOPIC) {}

void SensorDHT::Begin() {
    dht.begin();
    Serial.println("DHT sensor initialized");
}

float SensorDHT::GetTemperature() {
    return dht.readTemperature();
}

void SensorDHT::UpdateTemperature() {
    float currentTemperature = GetTemperature();
    if (!isnan(currentTemperature) && ((currentTemperature - lastTemperature)>= 1.00) || ((currentTemperature - lastTemperature) <= -1.00)) {
        lastTemperature = currentTemperature;
        outputDoc["state"]["reported"]["temperature"] = currentTemperature;
        serializeJson(outputDoc, outputBuffer);
        mqttHandler->Publish(UPDATE_TOPIC, outputBuffer);
        Serial.print("Temperature updated in shadow: ");
        Serial.println(currentTemperature);
    } else if (isnan(currentTemperature)) {
        Serial.println("Failed to read temperature from DHT sensor");
    } 
}
