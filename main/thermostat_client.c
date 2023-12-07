#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "temp_sensor.h"
#include "mdns_tasks.h"
#include "http_server.h"
#include "wifi_setup.h"

#define TEMP_SENSOR_PIN 13

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

    setup_wifi();

    TempSensoryMemory *temp_sensor_mem = malloc(sizeof(TempSensoryMemory));
    xTaskCreate(&temp_task, "Temp", 4096, temp_sensor_mem, 1, NULL);

    server = start_webserver();
    start_mdns();
}
