#include "esp_http_server.h"

// Basic Handle that returns "OK"
esp_err_t basic_handler(httpd_req_t* req){
    
    const char* resp = "This is OK!";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    
    return ESP_OK;
}
httpd_uri_t base_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = basic_handler,
    .user_ctx = NULL
};

// Server starter
httpd_handle_t start_webserver(void)
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &base_uri);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}
