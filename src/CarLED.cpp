#include "CarLED.h"

CarLED::CarLED() : ledState(LedStateEnum::off) {
    pinMode(LedPin,OUTPUT);
    Serial.println("LED is initialized");
} // Initialize the LED state as OFF

void CarLED::setLED(const char* state) {
    if(!strcmp(state, "on")){
        ledState = LedStateEnum::on;
        digitalWrite(LedPin,HIGH);
    }
    else if(!strcmp(state, "off")){
        ledState = LedStateEnum::off;
        digitalWrite(LedPin,LOW);
    }
}

LedStateEnum::ledState CarLED::getLED() const {
    return ledState;
}
