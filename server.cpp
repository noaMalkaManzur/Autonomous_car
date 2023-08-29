#include "server.h"

// global variables
int carSpeed = 35; 
int carDirection = STOP;

httpd_handle_t set_speed_httpd = NULL;
httpd_handle_t capture_httpd = NULL;
httpd_handle_t index_httpd = NULL;

httpd_handle_t lights_httpd = NULL;
httpd_handle_t backward_httpd = NULL;
httpd_handle_t forward_httpd = NULL;
httpd_handle_t left_httpd = NULL;
httpd_handle_t right_httpd = NULL;

// Handler functions




String getValue(String data, String key) {
    String separator = "&";
    String keyValueSeparator = "=";
    int keyIndex = data.indexOf(key + keyValueSeparator);

    if (keyIndex != -1) {
        int valueIndex = keyIndex + key.length() + keyValueSeparator.length();
        int endIndex = data.indexOf(separator, valueIndex);
        if (endIndex == -1) {
            endIndex = data.length();
        }

        return data.substring(valueIndex, endIndex);
    }

    return "";
}

static esp_err_t forward_handler(httpd_req_t *req) {
  carDirection = FORWARD;
  // Set the HTTP response headers
  httpd_resp_set_type(req, "text/plain");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*"); // Add CORS header if needed

  // Send a response message
  const char *response_message = "Direction changed to forward.";
  httpd_resp_sendstr(req, response_message);
  
  return ESP_OK;
}

static esp_err_t backward_handler(httpd_req_t *req) {
  carDirection = BACKWARD;
  // Set the HTTP response headers
  httpd_resp_set_type(req, "text/plain");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*"); // Add CORS header if needed

  // Send a response message
  const char *response_message = "Direction changed to backward.";
  httpd_resp_sendstr(req, response_message);
  
  return ESP_OK;
}

static esp_err_t left_handler(httpd_req_t *req) {
  carDirection = LEFT;
  // Set the HTTP response headers
  httpd_resp_set_type(req, "text/plain");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*"); // Add CORS header if needed

  // Send a response message
  const char *response_message = "Direction changed to left.";
  httpd_resp_sendstr(req, response_message);
  
  return ESP_OK;
}

static esp_err_t right_handler(httpd_req_t *req) {
  carDirection = RIGHT;
  // Set the HTTP response headers
  httpd_resp_set_type(req, "text/plain");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*"); // Add CORS header if needed

  // Send a response message
  const char *response_message = "Direction changed to right.";
  httpd_resp_sendstr(req, response_message);
  
  return ESP_OK;
}

static esp_err_t lights_handler(httpd_req_t *req) {
    //TO DO
}

static size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len){
    jpg_chunking_t *j = (jpg_chunking_t *)arg;
    if(!index){
        j->len = 0;
    }
    if(httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK){
        return 0;
    }
    j->len += len;
    return len;
}

void startCameraServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t stop_uri = {
    .uri       = "/stop",
    .method    = HTTP_GET,
    .handler   = stop_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t forward_uri = {
    .uri       = "/forward",
    .method    = HTTP_GET,
    .handler   = forward_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t backward_uri = {
    .uri       = "/backward",
    .method    = HTTP_GET,
    .handler   = backward_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t left_uri = {
    .uri       = "/left",
    .method    = HTTP_GET,
    .handler   = left_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t right_uri = {
    .uri       = "/right",
    .method    = HTTP_GET,
    .handler   = right_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t capture_uri = {
    .uri       = "/capture",
    .method    = HTTP_GET,
    .handler   = capture_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t set_speed_uri = {
    .uri       = "/set_speed",
    .method    = HTTP_GET,
    .handler   = set_speed_handler,
    .user_ctx  = NULL
  };

  if (httpd_start(&index_httpd, &config) == ESP_OK) {
        // Register URI handlers
        httpd_register_uri_handler(index_httpd, &index_uri); // Assuming index_uri is already defined
        httpd_register_uri_handler(index_httpd, &capture_uri);
        httpd_register_uri_handler(index_httpd, &set_speed_uri);
        httpd_register_uri_handler(index_httpd, &right_uri);
        httpd_register_uri_handler(index_httpd, &left_uri);
        httpd_register_uri_handler(index_httpd, &forward_uri);
        httpd_register_uri_handler(index_httpd, &backward_uri);
        httpd_register_uri_handler(index_httpd, &stop_uri);
        // httpd_register_uri_handler(index_httpd, &lights_uri);

        // Serial.printf("Starting web server on port: '%d'\n", config.server_port);
    } else {
        // Serial.println("Error starting server");
    }
}




