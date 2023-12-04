#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "temp_sensor.h"
#include "mdns_tasks.h"

#define TEMP_SENSOR_PIN 13

ds18b20_device_handle_t *temp_sensor;

void app_main(void)
{
    temp_sensor = malloc(sizeof(ds18b20_device_handle_t));
    get_single_temp_sensor(TEMP_SENSOR_PIN, temp_sensor);

    while(1){
        print_temp(*temp_sensor);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
