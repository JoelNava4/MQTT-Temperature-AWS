#include "MotorController.h"
#include <Arduino.h>

MotorController::MotorController(int pin, MQTTHandler* mqttHandler, const char* UPDATE_TOPIC)
    : motorPin(pin), mqttHandler(mqttHandler), UPDATE_TOPIC(UPDATE_TOPIC) {}

void MotorController::Begin() {
    pinMode(motorPin, OUTPUT);
    digitalWrite(motorPin, LOW); 
    Serial.println("Motor initialized and set to OFF");
    ReportState();
}

void MotorController::SetMotorState(int state) {
    if (motorState != state) {
        motorState = state;
        digitalWrite(motorPin, motorState ? HIGH : LOW);
        if (motorState) {
            Serial.println("Motor is ON");
        } else {
            Serial.println("Motor is OFF");
        }
        ReportState();
    }
}

void MotorController::ReportState() {
    outputDoc["state"]["reported"]["builtInmotor"] = motorState;
    serializeJson(outputDoc, outputBuffer);
    mqttHandler->Publish(UPDATE_TOPIC, outputBuffer);
    Serial.println("Motor state reported to MQTT");
}

void MotorController::HandleIncomingMessage(const String& MESSAGE) {
    StaticJsonDocument<128> inputDoc;
    DeserializationError error = deserializeJson(inputDoc, MESSAGE);
    if (!error) {
        int newState = inputDoc["state"]["builtInmotor"].as<int>();
        SetMotorState(newState);
    } else {
        Serial.println("Failed to parse JSON message for motor state");
    }
}
