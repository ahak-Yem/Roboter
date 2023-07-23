#include "MotorsManager.h"

const int MotorsManager::left_Back = LB;
const int MotorsManager::left_Forward = LF;
const int MotorsManager::right_Back = RB;
const int MotorsManager::right_Forward = RF;

MotorsManager::MotorsManager(){
    pinMode(left_Back,OUTPUT);
    pinMode(left_Forward,OUTPUT);
    pinMode(right_Back,OUTPUT);
    pinMode(right_Forward,OUTPUT);
}
bool MotorsManager::controlCar(const char* variable) {
    if (!strcmp(variable, "forward")) {
        Serial.println("Forward");
        digitalWrite(right_Forward, HIGH);
        digitalWrite(left_Back, LOW);
        digitalWrite(left_Forward, HIGH);
        digitalWrite(right_Back, LOW);
        return true;
    }
    else if (!strcmp(variable, "right")) {
        Serial.println("Right");
        digitalWrite(right_Forward, HIGH);
        digitalWrite(left_Back, HIGH);
        digitalWrite(left_Forward, LOW);
        digitalWrite(right_Back, LOW);
        return true;
    }
    else if (!strcmp(variable, "left")) {
        Serial.println("Left");
        digitalWrite(right_Forward, LOW);
        digitalWrite(left_Back, LOW);
        digitalWrite(left_Forward, HIGH);
        digitalWrite(right_Back, HIGH);
        return true;
    }
    else if (!strcmp(variable, "backward")) {
        Serial.println("Backward");
        digitalWrite(right_Forward, LOW);
        digitalWrite(left_Back, HIGH);
        digitalWrite(left_Forward, LOW);
        digitalWrite(right_Back, HIGH);
        return true;
    }
    else if (!strcmp(variable, "stop")) {
        Serial.println("Stop");
        digitalWrite(right_Forward, LOW);
        digitalWrite(left_Back, LOW);
        digitalWrite(left_Forward, LOW);
        digitalWrite(right_Back, LOW);
        return true;
    }
    else{
        return false;
    }
}
