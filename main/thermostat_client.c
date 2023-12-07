#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "temp_sensor.h"
#include "mdns_tasks.h"
#include "http_server.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#define TEMP_SENSOR_PIN 13
#define SSID "NARP House"
#define PASSWORD "slant3dR00f"

ds18b20_device_handle_t *temp_sensor;

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
