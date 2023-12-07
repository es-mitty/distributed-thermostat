#include "mdns.h"

void start_mdns(){
    ESP_ERROR_CHECK(mdns_init());
    mdns_hostname_set("thermostat");
    mdns_instance_name_set("IoT Thermostat");
}