#include "HttpServerHandler.h"
#include "esp_camera.h"
#include "Arduino.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
// #include <WiFi.h>
// #include "img_converters.h"
// #include "esp_timer.h"
// #include "fb_gfx.h"


//Constructor with a code to disable brownout detecting
HttpServerHandler::HttpServerHandler()
{
      WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
}


//A motorManager instance that is accessed in the cmd_handler to control the car
MotorsManager HttpServerHandler::motorManager;

//Configurations for the stream
#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

//A variable with HTML, CSS and JavaScript for frontpage style & actions capturing.
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>Ninja Auto</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      table { margin-left: auto; margin-right: auto; }
      td { padding: 8 px; }
      .circle-button {
		display: inline-block;
		width: 50px;
		height: 50px;
        background-color: #0487de;
        border-radius: 50%;
        color: white;
        text-align: center;
        font-size: 20px;
        margin: 6px;
		line-height: 50px;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }
	  .circle-button i{
	  transform: rotate(45deg);
	  }
      img {  width: auto ;
        max-width: 100% ;
        height: auto ; 
      }
    </style>
  </head>
  <body>
    <h1>Ninja Auto</h1>
    <img src="" id="photo" >
    <table>
      <tr>
	  <td colspan="3" align="center">
	  <div class="circle-button" onmousedown="toggleCheckbox('forward');" ontouchstart="toggleCheckbox('forward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">
	  <i class="fas fa-arrow-up"></i>
	  </div>
	  </td>
	  </tr>
      <tr><td align="center">
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
    </table>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/js/all.min.js"></script>
   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/action?go=" + x, true);
     xhr.send();
   }
   window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
  </script>
  </body>
</html>
)rawliteral";

//Handles the frontend that will open in main server
esp_err_t HttpServerHandler::indexHandler(httpd_req_t *req)
{
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

//Handles the stream in a second stream server with different port
esp_err_t HttpServerHandler::streamHandler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
  }
  return res;
}

//Handles the commands that comes from the main server
esp_err_t HttpServerHandler::cmdHandler(httpd_req_t *req)
{
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
  
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if(!buf){
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "go", variable, sizeof(variable)) == ESP_OK) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  sensor_t * s = esp_camera_sensor_get();
  int res = 0;
  
if(!motorManager.controlCar(variable)){
    res = -1;
}
  if(res){
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}


void HttpServerHandler::startServer()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = indexHandler,
    .user_ctx  = NULL
  };

  httpd_uri_t cmd_uri = {
    .uri       = "/action",
    .method    = HTTP_GET,
    .handler   = cmdHandler,
    .user_ctx  = NULL
  };
  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = streamHandler,
    .user_ctx  = NULL
  };
  if (httpd_start(&cameraHttpd, &config) == ESP_OK) {
    httpd_register_uri_handler(cameraHttpd, &index_uri);
    httpd_register_uri_handler(cameraHttpd, &cmd_uri);
  }
  config.server_port += 1;
  config.ctrl_port += 1;
  if (httpd_start(&streamHttpd, &config) == ESP_OK) {
    httpd_register_uri_handler(streamHttpd, &stream_uri);
  }
}