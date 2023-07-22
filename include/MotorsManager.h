#ifndef MOTORS_MANAGER_H
#define MOTORS_MANAGER_H

#include "Arduino.h"

#define LB   2
#define LF    14
#define RB  15
#define RF   13


class MotorsManager {
public:
    static const int left_Back;
    static const int left_Forward;
    static const int right_Back;
    static const int right_Forward;

    void controlCar(const char* variable);
    MotorsManager();
};

#endif
