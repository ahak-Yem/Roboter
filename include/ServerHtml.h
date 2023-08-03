#ifndef Server_HTML_H
#define Server_HTML_H

#include <Arduino.h>
namespace NinjaHtml{
//A variable with HTML, CSS and JavaScript for frontpage style & actions capturing.
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Ninja Auto</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css">
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin: 0;
            padding: 0;
        }

        #header {
            background-color: #0983b8;
            color: white;
            padding: 10px;
        }

        #container {
            display: flex;
            justify-content: center;
            align-items: center;
            flex-wrap: wrap;
        }

        #video-container {
            flex: 1;
            max-width: 100%;
            padding: 10px;
        }

        #button-container {
            flex: 1;
            max-width: 100%;
            padding: 10px;
        }

        table {
            margin-left: auto;
            margin-right: auto;
        }

        td {
            padding: 8px;
        }

        .circle-button {
            display: inline-block;
            width: 80px;
            height: 80px;
            background-color: #0983b8;
            border-radius: 50%;
            color: white;
            text-align: center;
            font-size: 20px;
            margin: 6px;
            line-height: 80px;
            cursor: pointer;
            -webkit-touch-callout: none;
            -webkit-user-select: none;
            -khtml-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
        }

        .circle-button i {
            transform: rotate(45deg);
        }

        img {
            width: auto;
            max-width: 100%;
            height: auto;
            transform: rotate(90deg);
        }
    </style>
</head>
<body>
    <div id="header">
        <h1>Ninja Auto</h1>
    </div>
    <div id="container">
        <div id="video-container">
            <img src="" id="photo">
        </div>
        <div id="button-container">
            <table>
                <tr>
                    <td colspan="3" align="center">
                        <div class="circle-button" onmousedown="toggleCheckbox('forward');" ontouchstart="toggleCheckbox('forward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">
                            <i class="fas fa-arrow-up"></i>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td align="center">
                        <div class="circle-button" onmousedown="toggleCheckbox('left');" ontouchstart="toggleCheckbox('left');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">
                            <i class="fas fa-arrow-left"></i>
                        </div>
                    </td>
                    <td align="center">
                        <div class="circle-button" onmousedown="toggleCheckbox('stop');" ontouchstart="toggleCheckbox('stop');">
                            <i class="fas fa-stop"></i>
                        </div>
                    </td>
                    <td align="center">
                        <div class="circle-button" onmousedown="toggleCheckbox('right');" ontouchstart="toggleCheckbox('right');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">
                            <i class="fas fa-arrow-right"></i>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td colspan="3" align="center">
                        <div class="circle-button" onmousedown="toggleCheckbox('backward');" ontouchstart="toggleCheckbox('backward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">
                            <i class="fas fa-arrow-down"></i>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td colspan="3" align="center">
                        <div class="circle-button" onclick="toggleLED('on');">
                            LED On
                        </div>
                    </td>
                    <td></td>
                    <td colspan="3" align="center">
                        <div class="circle-button" onclick="toggleLED('off');">
                            LED Off
                        </div>
                    </td>
                </tr>
            </table>
        </div>
    </div>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/js/all.min.js"></script>
    <script>
        function toggleCheckbox(x) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/action?go=" + x, true);
            xhr.send();
        }
        function toggleLED(status) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/led?led=" + status, true);
            xhr.send();
        }
        
        window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
    </script>
</body>
</html>
)rawliteral";
}
#endif // Server_HTML_H
