#ifndef SENSORDHT_H
#define SENSORDHT_H

#include "DHT.h"
#include "MQTTHandler.h"
#include <ArduinoJson.h>

class SensorDHT {
  private:
    int dhtPin;
    float lastTemperature;
    DHT dht;
    MQTTHandler* mqttHandler;
    const char* UPDATE_TOPIC;

    StaticJsonDocument<128> outputDoc;
    char outputBuffer[128];

  public:
    SensorDHT(int pin, int type, MQTTHandler* mqttHandler, const char* UPDATE_TOPIC);
    void Begin();
    void UpdateTemperature();
    float GetTemperature();
};

#endif // SENSORDHT_H
