#ifndef CAR_LED_H
#define CAR_LED_H

#include <Arduino.h>

#define LedPin 4

namespace LedStateEnum{
    enum ledState{
        on,
        off
    };
}
class CarLED {
public:
    CarLED(); // Constructor

    void setLED(const char* state); // Function to set the LED state (true for ON, false for OFF)
    LedStateEnum::ledState getLED() const; // Function to get the current LED state

private:
    LedStateEnum::ledState ledState; // Variable to store the LED state (true for ON, false for OFF)
};

#endif // CAR_LED_H
