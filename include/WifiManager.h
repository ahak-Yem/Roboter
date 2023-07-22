#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <Arduino.h>

class WifiManager{

public:
    WifiManager(const char* ssid, const char* password);
    bool connect();
    IPAddress getIPAddress();
private:
    const char* _ssid;
    const char* _password;
};

#endif