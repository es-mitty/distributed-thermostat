#include <stdlib.h>
#include "cJSON.h"
#include "string.h"

typedef struct thermo_data_t{
    char node_name[30];
    float target_temp;
    float measured_temp;
};

typedef struct thermo_sys_t{
    char system_name[30];
    int num_sensors;
    thermo_data_t *zones;
};

void init_thermo_data(thermo_sys_t *tdat_handle){
    
    strcpy(tdat_handle->system_name, "Example Household");
    tdat_handle->num_sensors = 1;

    thermo_data_t sensor_1;
    thermo_data_t *sensors = (thermo_data_t *)malloc(sizeof(thermo_data_t) * tdat_handle->num_sensors);
    strcpy(sensor_1.node_name, "Thermostat 1");

}

void data_ingest(char* json_string){
    
}

char* data_export(){
    return "";
}