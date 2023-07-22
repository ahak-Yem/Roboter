#include <Arduino.h>
#include "WifiManager.h"
#include "CameraManager.h"

const char* ssid = "Yemen";
const char* password = "123456789";
WifiManager wifiManager(ssid, password);//init WifiManager
CameraManager camManager; //init CameraManager

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(500);

    if (camManager.initCamera()) {
        Serial.println("Camera initialized");
    } else {
        Serial.println("Camera initialization failed");
    }

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
