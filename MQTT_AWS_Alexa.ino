#include <Arduino.h>
#include "WiFiConnection.h"
#include "MQTTHandler.h"
#include "MotorController.h"
#include "SensorDHT.h"

const char* WIFI_SSID = "jota";
const char* WIFI_PASS = "joelnava4";
const char* MQTT_BROKER = "a1wpqxeafukdx9-ats.iot.us-east-1.amazonaws.com";
const int MQTT_PORT = 8883;
const char* CLIENT_ID = "ESP-32";
const char* UPDATE_TOPIC = "$aws/things/motorTemperature/shadow/update";
const char* UPDATE_DELTA_TOPIC = "$aws/things/motorTemperature/shadow/update/delta";

const int MOTOR_PIN = 5;
const int DHT_PIN = 4;
const int DHT_TYPE = DHT22;

const char AMAZON_ROOT_CA1[] PROGMEM = R"EOF(-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----)EOF";

const char CERTIFICATE[] PROGMEM = R"KEY(-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUUatA6f4XlDRBVLmTKnlhcCsZw20wDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MTEwNzIwNTgx
MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAJtoDoOLrT8hKoaCzoiX
mcRjj+L0b1sqGpKhUBOfY+DnYi/ixex9Sb5cHsGKmF/rRDGqsTMy+WqeE94NLulC
mwE1SGaJQ7IQkkYZGo3zi3b3fFU4hEE7an8PwF79bNopojZgV0mP4PHEG2l5RqFB
o97GDsWsPV9/AkkfLCa8wiAJp0Va9iwMWJIwHPaQvr16xspsPz5XpE1R/q7g3kem
uE3TNzVGHNjbSDViGYQVm6n9GseUi3mzt4hdSnxC4VAWW593Ol5PzatLrVhAIH6h
dYKVFUvm2Buv01sF56YXAZxvEZ2wXUAodbpUbc8AIb58H3jaSNI/AP5D+HoJul7a
h0kCAwEAAaNgMF4wHwYDVR0jBBgwFoAUKezosvTFtbqK6gP5vt57uFGAHGQwHQYD
VR0OBBYEFNAxtU3bR2BgTt2RcjeYpz8ooeC1MAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCDgtUgabXbSWQYIu0d3JsD2UGx
k1eBOnl1OXqv2yozZtAGiJXzywKnX+vtmXwetnYm41Ls1ciook+Um0P/PtOxQi42
YeAz13OHvRoR9r0RjgBTE/yKFe38ogV3GC8lhKceoStJgD1GGo92qLp8jkgz8E4S
zo8lZnPSCtSqMDjHvsJFkLb0xqis/aZ7aSTL2cyZDgqjT6JXwjYqNvtkHExFlcIN
Kpwl1LJg92QamCGg+j/tT9RUDmWbKbFGJCj9AnjVbBliwDxoDpNdpDC9nDsBXAy5
EUU8P6IGpiavYCRoojgS1bex1nxN1UkTucZ5DdApgBSKed6RcI9hj5B+gszl
-----END CERTIFICATE-----
)KEY";

const char PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAm2gOg4utPyEqhoLOiJeZxGOP4vRvWyoakqFQE59j4OdiL+LF
7H1JvlwewYqYX+tEMaqxMzL5ap4T3g0u6UKbATVIZolDshCSRhkajfOLdvd8VTiE
QTtqfw/AXv1s2imiNmBXSY/g8cQbaXlGoUGj3sYOxaw9X38CSR8sJrzCIAmnRVr2
LAxYkjAc9pC+vXrGymw/PlekTVH+ruDeR6a4TdM3NUYc2NtINWIZhBWbqf0ax5SL
ebO3iF1KfELhUBZbn3c6Xk/Nq0utWEAgfqF1gpUVS+bYG6/TWwXnphcBnG8RnbBd
QCh1ulRtzwAhvnwfeNpI0j8A/kP4egm6XtqHSQIDAQABAoIBAQCVh8snUbdyHeSA
Js65aT2AVNg25UFcc6e73NFjACOnXJwCPW55/Zd+Pb8uNMAm0wX70CTOgcaxisbN
CtxPf1IMGIvAdWUYqY9FOZRub5+K6djNWyMObTAscCiGfcxGInc9YPe8BV/I4QwL
MbD/KlyV1vaRxwgKofe+Cet+nnMfRQqkb2isctT/9g27bwDgNxWuFFLOdQl+O43t
K63UZhU0Gm+2dylebJLmnRNOTj/iQt2YpbORuVXSRtL7FPTpaemJC3D/gG45uriG
zPe4iqKadSlxATmVW/214vhDa78ve/nnPeDC0hWEO5TO/bJKtooaWLFyDtg9WgHW
6d73t9wBAoGBAMxeEj7Kqfrq284LHD9ZsE7n1kqt65+x35RedL8h9BWv5zSCNArK
HosSUNXRh+BeSyrBFQOHg5MBFKFUjeDgLgERbITOF1kYnlbXOGBtRicPCZswyBn9
ECUKGBDNBwQ0DwpE7B+kh5/owuDhbxvMy61Z8GQpV4LHni6Bu4/HXoqBAoGBAMKr
UnzQ3pDwxusr0pFHrHONgLUWdRJBOm535Gscow07UUvne1CKvJZYi7vC4i43Hx+g
kgByttQtNG4m5j/1Jbpz+YMgQbAuRN1joMQXyzxO5yxxgfFe715IrVBdceH9318Z
oZMv+mP/92M9o0U3fgNUBfmUk4NYGCp3DM9V68jJAoGBAKSTySup95MqmVJnnhri
NlqE1wyS3OJg0wVfv7LAPBlO6+lcRV6/ADPMzy4xq7OjG/NVRH/wHKRAxYVaW1tS
DZoslvCoKyND8RdMAIme1lTUBk9kzHApXFwHsfOtuM3IwqfpcyHUVKgRQQbqZJjF
pux2NmHi7SdTa+5V6V7TFi6BAoGAOi/FTyOLcx1VB9Qoc15OEJf6IaadxaATnBNg
BhggMINLcHm3ZUKizfmXgr7kplW5itWO7m1bcp40brxX1EQtS8cAjI4mtEeQM/Bk
jFhkWhlnheRDp+bte2wTydK0iI0+qAlcP0d2+072Hga9d6H3m30Q8CHV2cuKJZ9C
B/Mxr+kCgYEAjYx0SE91EzrFJiOihmEPfbhCX1OfTb+XIdJET+sa4VJhwjXptwfo
WvVJ4/mMmaSUcr1lMup42UDWwppLZYlWDyRSnsUwV6n/mgHQ/Qyx8dLqajxeBPOu
Vx5kFWEKF9/7o+6jNqegSMkkY+0VX9PDGzBYUFbyDeT509wNm5mxdw8=
-----END RSA PRIVATE KEY-----
)KEY";

WiFiConnection wifi(WIFI_SSID, WIFI_PASS);
MQTTHandler mqttHandler(MQTT_BROKER, MQTT_PORT, CLIENT_ID, UPDATE_TOPIC, UPDATE_DELTA_TOPIC);
MotorController motorController(MOTOR_PIN, &mqttHandler, UPDATE_TOPIC);
SensorDHT sensorDHT(DHT_PIN, DHT_TYPE, &mqttHandler, UPDATE_TOPIC);

void callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("Message received on topic " + String(topic) + ": " + message);

    if (String(topic) == UPDATE_DELTA_TOPIC) {
        motorController.HandleIncomingMessage(message);
    }
}

void setup() {
    Serial.begin(115200);
    wifi.Connect();
    mqttHandler.SetCertificates(AMAZON_ROOT_CA1, CERTIFICATE, PRIVATE_KEY);
    mqttHandler.SetCallback(callback);
    mqttHandler.Connect();

    motorController.Begin();
    sensorDHT.Begin();
}

void loop() {
    if (!mqttHandler.IsConnected()) {
        mqttHandler.Connect();
    }
    mqttHandler.Loop();
    sensorDHT.UpdateTemperature();

    delay(5000); 
}