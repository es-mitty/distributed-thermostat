#include "onewire_bus.h"
#include "ds18b20.h"


typedef struct{
    // Onewire variables
    onewire_bus_config_t bus_config;
    onewire_bus_rmt_config_t rmt_config;
    onewire_bus_handle_t bus_handle;
    onewire_device_iter_handle_t device_iter;
    onewire_device_t device;
    // DS18B20 variables
    ds18b20_config_t sensor_config;
    ds18b20_device_handle_t temp_sensor_handle;
} TempSensoryMemory;

void print_temp(ds18b20_device_handle_t device_handle);
float to_fahrenheit(float from_celsius);

esp_err_t get_single_temp_sensor(int gpio_pin, TempSensoryMemory *sensor_struct){
    // Init bus
    ESP_ERROR_CHECK(onewire_new_bus_rmt(&(sensor_struct->bus_config), &(sensor_struct->rmt_config), &(sensor_struct->bus_handle)));
    // Get first device
    ESP_ERROR_CHECK(onewire_new_device_iter(sensor_struct->bus_handle, &(sensor_struct->device_iter)));
    ESP_ERROR_CHECK(onewire_device_iter_get_next(sensor_struct->device_iter, &(sensor_struct->device)));
    // Convert to DS18B20
    ESP_ERROR_CHECK(ds18b20_new_device(&(sensor_struct->device),&(sensor_struct->sensor_config),&(sensor_struct->temp_sensor_handle)));
    // Print a temp
    print_temp(sensor_struct->temp_sensor_handle);

    return ESP_OK;
}

void print_temp(ds18b20_device_handle_t device_handle){
    ds18b20_trigger_temperature_conversion(device_handle);
    float temp = 0;
    ds18b20_get_temperature(device_handle, &temp);
    printf("Temperature is %.5f°F.\n", to_fahrenheit(temp));
}

float to_fahrenheit(float from_celsius){
    return (from_celsius * 9.f / 5.f) + 32.f;
}