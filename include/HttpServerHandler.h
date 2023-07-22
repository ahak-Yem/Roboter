#ifndef HTTP_SERVER_HANDLER_H
#define HTTP_SERVER_HANDLER_H

#include "esp_http_server.h"
#include <Arduino.h>
#include "MotorsManager.h"
class HttpServerHandler{
    public:
    HttpServerHandler();
    ~HttpServerHandler();
    void startServer();

    private:
    httpd_handle_t cameraHttpd = NULL;
    httpd_handle_t streamHttpd = NULL; // Two servers for the stream and main one

    static MotorsManager motorManager;    
    static esp_err_t indexHandler(httpd_req_t *req); //handles frontend
    static esp_err_t streamHandler(httpd_req_t *req); //handles stream
    static esp_err_t cmdHandler(httpd_req_t *req); //handles commands comming
};

#endif