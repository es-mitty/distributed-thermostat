#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "temp_sensor.h"
#include "mdns_tasks.h"

#define TEMP_SENSOR_PIN 13

void app_main(void)
{
    ds18b20_device_handle_t handle;
    get_single_temp_sensor(TEMP_SENSOR_PIN, &handle);

    while(1){
        print_temp(handle);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}
