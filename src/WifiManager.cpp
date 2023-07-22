#include "WifiManager.h"

//Constructor to set id & password.
WifiManager::WifiManager(const char* ssid, const char* password) {
    this->_ssid = ssid;
    this->_password = password;
}

//A method to start wifi connection with handling failure.
bool WifiManager::connect() {
    WiFi.begin(this->_ssid, this->_password);

    int retries = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        retries++;
        if (retries > 20) {
            return false; // Failed to connect after 20 seconds
        }
    }
    return true;
}

//Return the IP address of the ESP32
IPAddress WifiManager::getIPAddress() {
    return WiFi.localIP();
}
