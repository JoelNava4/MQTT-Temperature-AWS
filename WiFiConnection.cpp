#include "WiFiConnection.h"

WiFiConnection::WiFiConnection(const char* SSID, const char* PASSWORD) : SSID(SSID), PASSWORD(PASSWORD) {}

void WiFiConnection::Connect() {
    WiFi.begin(SSID, PASSWORD);
    Serial.println("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
