#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "temp_sensor.h"
#include "mdns_tasks.h"
#include "http_server.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"

#define TEMP_SENSOR_PIN 13
#define SSID "NARP House"
#define PASSWORD "slant3dR00f"
static const char *TAG = "WIFI MODULE";
static int s_retry_num = 0;

ds18b20_device_handle_t *temp_sensor;

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
    }
}

void temp_task(void *args){
    TempSensoryMemory *sensor_mem = (TempSensoryMemory*)(args);

    sensor_mem->bus_config.bus_gpio_num = TEMP_SENSOR_PIN;
    sensor_mem->rmt_config.max_rx_bytes = 10;

    get_single_temp_sensor(TEMP_SENSOR_PIN, sensor_mem);

    while(1){
        print_temp(sensor_mem->temp_sensor_handle);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{

    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t winit_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&winit_config));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = SSID,
            .password = PASSWORD
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    TempSensoryMemory *temp_sensor_mem = malloc(sizeof(TempSensoryMemory));
    xTaskCreate(&temp_task, "Temp", 4096, temp_sensor_mem, 1, NULL);

    server = start_webserver();
}
