#include "HttpServerHandler.h"

//Constructor with a code to disable brownout detecting
HttpServerHandler::HttpServerHandler()
{
      WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
}

//A motorManager instance that is accessed in the cmd_handler to control the car
MotorsManager HttpServerHandler::motorManager;
CarLED HttpServerHandler::led;

//Configurations for the stream
#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
ra_filter_t HttpServerHandler::ra_filter;

//Handles the frontend that will open in main server
esp_err_t HttpServerHandler::indexHandler(httpd_req_t *req)
{
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)NinjaHtml::INDEX_HTML, strlen(NinjaHtml::INDEX_HTML));
}

//Handles the stream in a second stream server with different port
esp_err_t HttpServerHandler::streamHandler(httpd_req_t *req){
camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    char * part_buf[64];

    static int64_t last_frame = 0;
    if(!last_frame) {
        last_frame = esp_timer_get_time();
    }

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        return res;
    }

    while(true){
        fb = esp_camera_fb_get();
        if (!fb) {
            Serial.printf("Camera capture failed");
            res = ESP_FAIL;
        } else {
            if(fb->format != PIXFORMAT_JPEG){
                bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
                esp_camera_fb_return(fb);
                fb = NULL;
                if(!jpeg_converted){
                    Serial.printf("JPEG compression failed");
                    res = ESP_FAIL;
                }
            } else {
                _jpg_buf_len = fb->len;
                _jpg_buf = fb->buf;
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
        int64_t fr_end = esp_timer_get_time();

        int64_t frame_time = fr_end - last_frame;
        last_frame = fr_end;
        frame_time /= 1000;
        uint32_t avg_frame_time = ra_filter_run(&ra_filter, frame_time);
        Serial.printf("MJPG: %uB %ums (%.1ffps), AVG: %ums (%.1ffps)"
            ,(uint32_t)(_jpg_buf_len),
            (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time,
            avg_frame_time, 1000.0 / avg_frame_time
        );  //Remove this for a better serial monitor output. Used to analyze stream smoothness.
    }

    last_frame = 0;
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

esp_err_t HttpServerHandler::ledHandler(httpd_req_t *req) {
    char* buf;
    size_t buf_len;
    char variable[32] = {0,};
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = (char*)malloc(buf_len);
        if (!buf) {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            if (httpd_query_key_value(buf, "led", variable, sizeof(variable)) == ESP_OK) {
                Serial.print("Received LED status: ");
                Serial.println(variable);
                led.setLED(variable);
                free(buf);
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
    } else {
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    return httpd_resp_send(req, NULL, 0);
}


//Used to initialize a moving average filter to smooth out a stream of values.
ra_filter_t *HttpServerHandler::ra_filter_init(ra_filter_t *filter, size_t sample_size)
{
    memset(filter, 0, sizeof(ra_filter_t));

    filter->values = (int *)malloc(sample_size * sizeof(int));
    if(!filter->values){
        return NULL;
    }
    memset(filter->values, 0, sample_size * sizeof(int));

    filter->size = sample_size;
    return filter;
}

//Apply a moving average filter to a stream of values. 
//It updates the filter's internal circular buffer with new values, 
//removes the oldest value, and calculates the average of the current values in the buffer. 
int HttpServerHandler::ra_filter_run(ra_filter_t *filter, int value)
{
  if(!filter->values){
    return value;
  }
  filter->sum -= filter->values[filter->index];
  filter->values[filter->index] = value;
  filter->sum += filter->values[filter->index];
  filter->index++;
  filter->index = filter->index % filter->size;
  if (filter->count < filter->size) {
    filter->count++;
  }
  return filter->sum / filter->count;
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
   httpd_uri_t led_uri = {
        .uri       = "/led",
        .method    = HTTP_GET,
        .handler   = ledHandler,
        .user_ctx  = NULL
    };
  ra_filter_init(&ra_filter, 20);
  Serial.printf("Starting web server on port: '%d'", config.server_port);
  if (httpd_start(&cameraHttpd, &config) == ESP_OK) {
    httpd_register_uri_handler(cameraHttpd, &index_uri);
    httpd_register_uri_handler(cameraHttpd, &cmd_uri);
    httpd_register_uri_handler(cameraHttpd, &led_uri);
  }
  config.server_port += 1;
  config.ctrl_port += 1;
  if (httpd_start(&streamHttpd, &config) == ESP_OK) {
    httpd_register_uri_handler(streamHttpd, &stream_uri);
  }
}