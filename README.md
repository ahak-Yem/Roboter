![Ninja Auto Logo](https://i.ibb.co/zVn76wL/Ninja-Auto-Logo.png)

# Ninja Auto
## Description
**Ninja Car** is a robot car with an interactive project that allows you to control a robot car using a browser-based interface. The car is equipped with an ESP32-Cam microcontroller, which serves as the brain of the operation. You can drive the robot car remotely using the hosted interface webpage, turn the built in LED on/off, and it even streams live video from the built-in camera to the same server. 

## Features
- Web-based control interface for driving the robot car.
- Control the robot car's LED remotely using the browser interface.
- Live video streaming from the robot's camera to the server.

## Installation
### Prerequisites
- ESP32-Cam board with appropriate firmware.
- A robot car with a L298n motor driver and four motors connected to it.
[Smart Robot ESP32 Cam](https://de.aliexpress.com/item/1005003474582419.html?spm=a2g0o.order_list.order_list_main.4.a7fe5c5fvbLImK&gatewayAdapt=glo2deu)

- VS Code with PlatformIO extension to be able to upload the software.

### Setup Steps
- Clone or download this repository to your local machine.

- Open this project using your preferred IDE (VS Code).

- Set the WiFi ssid & password in the source code main.cpp using your local personal Wifi data.
  ```
   //Personal WiFi data
   const char* ssid = "Yemen";
   const char* password = "123456789";
  ```

- Upload the source code to your ESP32 board using VS Code PlatformIO extension or Arduino IDE.

- Connect the robot motor driver and motors as shown in the circuit plan.

[![Alt Text](https://i.ibb.co/k5nn4CT/Screenshot-2023-06-21-at-01-50-39.png)](https://ibb.co/hL55BzN)

- Power on the robot car and make sure it is connected to the same network as your machine.

## Usage
1. Open your web browser and navigate to the esp32 server address.

2. You will see the web-based control interface with options to move the robot car forward, backward, left, right and stop it.

3. You will see the video streaming from the robot car's camera on the same webpage.

4. Control the robot car's LED by clicking on the "Turn On" and "Turn Off" buttons provided on the webpage.

## Configuration
If you need to modify any settings or pin assignments, refer to the appropriate sections in the source code.

## GPIO Pinout

| Purpose                   | GPIO Pin |
|---------------------------|----------|
| Right Motors forward      | GPIO 13  |
| Right Motors backward     | GPIO 15  |
| Left Motors forward       | GPIO 14  |
| Left Motors backward      | GPIO 2   |
| LED                       | GPIO 4   |
| PWDN (Camera)             | GPIO 32  |
| RESET (Camera)            | GPIO -1  |
| XCLK (Camera)             | GPIO 0   |
| SIOD (Camera)             | GPIO 26  |
| SIOC (Camera)             | GPIO 27  |
| Y9 (Camera)               | GPIO 35  |
| Y8 (Camera)               | GPIO 34  |
| Y7 (Camera)               | GPIO 39  |
| Y6 (Camera)               | GPIO 36  |
| Y5 (Camera)               | GPIO 21  |
| Y4 (Camera)               | GPIO 19  |
| Y3 (Camera)               | GPIO 18  |
| Y2 (Camera)               | GPIO 5   |
| VSYNC (Camera)            | GPIO 25  |
| HREF (Camera)             | GPIO 23  |
| PCLK (Camera)             | GPIO 22  |

## Contact
Email: ninja_auto_support@gmail.com

## Libraries
- esp32-camera: [GitHub repository](https://github.com/espressif/esp32-camera)

## Extras
- Our Qt Quick App to control the Robot Car: [Qt_Roboter_App][(https://gitlab.rz.htw-berlin.de/s0580976/ninjaauto_qtapp)](https://github.com/ahak-Yem/Qt_Roboter_App)

## Contributors
- Ahmed Kutbi
- Ahmed Rassea
- Heltonn Harold
- Mohammed Al-Hamdi

## License
[MIT License](https://en.wikipedia.org/wiki/MIT_License)

## Conclusion
Thank you for exploring our project. We hope you enjoy controlling the robot and watching its adventures through the live video stream. Have fun exploring and experimenting with the possibilities of this project!


[![Robot-Picture](https://i.ibb.co/4gnZ6NB/Robot-Picture.jpg)](https://ibb.co/1JtfD2H)
