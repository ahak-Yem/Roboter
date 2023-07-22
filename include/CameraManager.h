#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <esp_camera.h>
#include <Arduino.h>

//Cam Pins Defination
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define PART_BOUNDARY "123456789000000000000987654321" //TODO: Check if can be deleted

class CameraManager {
public:
    bool initCamera(); //Init the camera.
    CameraManager(); //Default constructor of the class
private:
};

#endif