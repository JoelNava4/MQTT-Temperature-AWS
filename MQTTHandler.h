#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class MQTTHandler {
  private:
    const char* MQTT_BROKER;
    const int MQTT_PORT;
    const char* CLIENT_ID;
    const char* UPDATE_TOPIC;
    const char* DELTA_TOPIC;
    WiFiClientSecure wifiClient;
    PubSubClient client;

  public:
    MQTTHandler(const char* MQTT_BROKER,const int MQTT_PORT, const char* CLIENT_ID, const char* UPDATE_TOPIC, const char* DELTA_TOPIC);
    void SetCertificates(const char* ROOT_CA, const char* CERTIFICATE, const char* PRIVATE_KEY);
    void Connect();
    void Subscribe();
    void Publish(const char* TOPIC, char* message);
    void Loop();
    void SetCallback(MQTT_CALLBACK_SIGNATURE);
    bool IsConnected();
};

#endif // MQTTHANDLER_H
