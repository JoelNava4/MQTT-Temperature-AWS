#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <WiFi.h>

class WiFiConnection {
  private:
    const char* SSID;
    const char* PASSWORD;

  public:
    WiFiConnection(const char* SSID, const char* PASSWORD);
    void Connect();
};

#endif // WIFICONNECTION_H
