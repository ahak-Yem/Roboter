#include <Arduino.h>
#include "WifiManager.h"
#include "CameraManager.h"
#include "HttpServerHandler.h"

//Personal WiFi data
const char* ssid = "Yemen";
const char* password = "123456789";

WifiManager wifiManager(ssid, password); //init WifiManager instance
CameraManager camManager; //init CameraManager instance
HttpServerHandler httpHandler; //init httpHandler instance

void setup() {
    Serial.begin(115200);
    delay(100);

    //Initialize the camera and retry 5 times if failed.
    int retryCount = 0;
    while (retryCount < 5) {
        if (camManager.initCamera()) {
            Serial.println("Camera initialized");
            break;
        } 
        else {
            Serial.println("Camera initialization failed");
            retryCount++;
            delay(500);
        }
    }

    //Connect to WiFi and retry 5 times if failed.
    retryCount = 0;
    while (retryCount < 5) {
        if (wifiManager.connect()) {
            Serial.println("Wi-Fi connected");
            Serial.print("IP address: ");
            Serial.println(wifiManager.getIPAddress());
            break;
        } 
        else {
            Serial.println("Failed to connect to Wi-Fi");
            retryCount++;
            delay(500);
        }
    }

    //Start the server that will controll the car
    httpHandler.startServer();
}

void loop() {
}
