#include "esp_http_server.h"
#include "esp_log.h"

static const char *HTTPTAG = "HTTP SERVER";

// Basic Handle that returns "OK"
esp_err_t basic_handler(httpd_req_t* req){
    
    const char* resp = "<!DOCTYPE html>\n<html lang=en>\n<head>\n<meta charset=UTF-8>\n<meta name=viewport content=\"width=device-width, initial-scale=1.0\">\n<title>Thermostat</title>\n</head>\n<body>\n</body>\n<script>body=document.body;test_json={\"system_name\":\"388 College St. #6\",\"zones\":[{\"name\":\"Eli\'s Room\",\"target_temp\":10.0,\"measured_temp\":9.8,},{\"name\":\"Luke\'s Room\",\"target_temp\":11.0,\"measured_temp\":10.8,},{\"name\":\"Living Room\",\"target_temp\":12.0,\"measured_temp\":11.8,},]};document.title=test_json.system_name;heading=document.createElement(\"h1\");heading.innerHTML=test_json.system_name;body.appendChild(heading);function changeItem(key,index,value){test_json.zones[index][key]=value;console.log(test_json);}\nfunction gen_input_elem(element,key,index,label){return`<label for=\"${key}\">${label}</label><input type=\"text\"name=\"${key}\"value=\"${element[key]}\"onchange=\"changeItem(${key}, ${index}, this.value)\"><br>`;}\nfunction refresh_ui(){test_json.zones.forEach((element,index,array)=>{elem=document.createElement(\"section\");elem.innerHTML+=gen_input_elem(element,\"name\",index,\"Name\");elem.innerHTML+=gen_input_elem(element,\"target_temp\",index,\"Desired Temp\");elem.innerHTML+=`Measured Temp:${element.measured_temp}`;body.appendChild(elem);});}\nrefresh_ui();async function getData(){response=await fetch(\"http://thermostat.local/data\");json=await response.text();if(json){\ntest_json=JSON.parse(json);refresh_ui();}};setTimeout(getData,1000);</script>\n</html>";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    
    return ESP_OK;
}

esp_err_t data_get(httpd_req_t* req){
    const char* resp = "{\"system_name\":\"388 College St. #6\",\"zone_len\":3,\"zones\":[{\"name\":\"Eli's Room\",\"id\":0,\"target_temp\":10,\"measured_temp\":9.8},{\"name\":\"Luke's Room\",\"id\":0,\"target_temp\":11,\"measured_temp\":10.8},{\"name\":\"Living Room\",\"id\":0,\"target_temp\":12,\"measured_temp\":11.8}]}";
    
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "Content-Type");

    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t data_post(httpd_req_t* req){

    //CORS
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "Content-Type");

    int req_len = req->content_len;
    char buff[req_len];
    httpd_req_recv(req, buff, req_len);
    ESP_LOGI(HTTPTAG, "Data: %s", buff);
    httpd_resp_set_status(req, HTTPD_200);
    httpd_resp_send(req, NULL, 0);
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

httpd_uri_t data_options_uri = {
    .uri = "/data",
    .method = HTTP_OPTIONS,
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
        httpd_register_uri_handler(server, &data_options_uri);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}
