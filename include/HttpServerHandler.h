#ifndef HTTP_SERVER_HANDLER_H
#define HTTP_SERVER_HANDLER_H

#include "esp_http_server.h"
#include <Arduino.h>
#include "MotorsManager.h"
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_timer.h"
#include "img_converters.h"


typedef struct {
        size_t size; //number of values used for filtering
        size_t index; //current value index
        size_t count; //value count
        int sum;
        int * values; //array to be filled with values
} ra_filter_t;

typedef struct {
        httpd_req_t *req;
        size_t len;
} jpg_chunking_t;



class HttpServerHandler{
    public:
    HttpServerHandler();
    void startServer();

    private:
    httpd_handle_t cameraHttpd = NULL;
    httpd_handle_t streamHttpd = NULL; // Two servers for the stream and the html.
    static ra_filter_t ra_filter;

    static MotorsManager motorManager;    
    static esp_err_t indexHandler(httpd_req_t *req); //handles frontend
    static esp_err_t streamHandler(httpd_req_t *req); //handles stream
    static esp_err_t cmdHandler(httpd_req_t *req); //handles commands comming
    static size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len);
    static ra_filter_t * ra_filter_init(ra_filter_t * filter, size_t sample_size); 
    static int ra_filter_run(ra_filter_t * filter, int value);
};

#endif