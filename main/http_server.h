#include "esp_http_server.h"

// Basic Handle that returns "OK"
esp_err_t basic_handler(httpd_req_t* req){
    
    const char* resp = "<iframe src='https://esmith87.w3.uvm.edu/' style='width:100vw; height:100vh;'/>";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    
    return ESP_OK;
}

esp_err_t data_get(httpd_req_t* req){
    const char* resp = "<p>Data get.</p>";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t data_post(httpd_req_t* req){

    return ESP_OK;
}

httpd_uri_t base_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = basic_handler,
    .user_ctx = NULL
};

httpd_uri_t data_get_uri = {
    .uri = "/data",
    .method = HTTP_GET,
    .handler = data_get,
    .user_ctx = NULL
};

httpd_uri_t data_post_uri = {
    .uri = "/data",
    .method = HTTP_POST,
    .handler = data_post,
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
        httpd_register_uri_handler(server, &data_get_uri);
        httpd_register_uri_handler(server, &data_post_uri);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}
