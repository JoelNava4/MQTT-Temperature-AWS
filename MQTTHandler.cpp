#include "MQTTHandler.h"

MQTTHandler::MQTTHandler(const char* MQTT_BROKER,const int MQTT_PORT, const char* CLIENT_ID, const char* UPDATE_TOPIC, const char* DELTA_TOPIC)
    : MQTT_BROKER(MQTT_BROKER), MQTT_PORT(MQTT_PORT), CLIENT_ID(CLIENT_ID), UPDATE_TOPIC(UPDATE_TOPIC), DELTA_TOPIC(DELTA_TOPIC), client(wifiClient) {
    client.setServer(MQTT_BROKER, MQTT_PORT);
}

void MQTTHandler::SetCertificates(const char* ROOT_CA, const char* CERTIFICATE, const char* PRIVATE_KEY) {
    wifiClient.setCACert(ROOT_CA);
    wifiClient.setCertificate(CERTIFICATE);
    wifiClient.setPrivateKey(PRIVATE_KEY);
}

void MQTTHandler::Connect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect(CLIENT_ID)) {
            Serial.println("connected");
            Subscribe();
            delay(100);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" trying again in 5 seconds");
            delay(5000);
        }
    }
}

void MQTTHandler::Subscribe() {
    client.subscribe(DELTA_TOPIC);
    Serial.println("Subscribed to " + String(DELTA_TOPIC));
}

void MQTTHandler::Publish(const char* TOPIC, char* message){
    client.publish(TOPIC, message);
}

void MQTTHandler::Loop() {
    client.loop();
}

void MQTTHandler::SetCallback(MQTT_CALLBACK_SIGNATURE) {
    client.setCallback(callback);
}

bool MQTTHandler::IsConnected() {
    return client.connected();
}
