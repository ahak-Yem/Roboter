#include <Arduino.h>
#include "WifiManager.h"

const char* ssid = "Yemen";
const char* password = "123456789";
WifiManager wifiManager(ssid, password);

void setup() {
    Serial.begin(115200);
    if (wifiManager.connect()) {
        Serial.println("Wi-Fi connected");
        Serial.print("IP address: ");
        Serial.println(wifiManager.getIPAddress());
    } else {
        Serial.println("Failed to connect to Wi-Fi");
    }
}

void loop() {
}
