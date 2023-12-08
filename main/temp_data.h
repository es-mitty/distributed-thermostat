#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include "string.h"

typedef struct {
    char node_name[30];
    double target_temp;
    double measured_temp;
} thermo_data_t;

typedef struct {
    char system_name[30];
    int num_sensors;
    thermo_data_t *zones;
} thermo_sys_t;

void init_thermo_data(thermo_sys_t *tdat_handle){

    strcpy(tdat_handle->system_name, "Example Household");
    tdat_handle->num_sensors = 1;

    thermo_data_t sensor_1;
    thermo_data_t *sensors = (thermo_data_t *)malloc(sizeof(thermo_data_t) * tdat_handle->num_sensors);
    strcpy(sensor_1.node_name, "Thermostat 1");

}

void data_ingest(char* json_string, thermo_sys_t* t_system_handle){
    cJSON *json = cJSON_Parse(json_string);

    char system_name[30];
    int num_sensors;
    const cJSON *j_zones = NULL;
    const cJSON *j_zone = NULL;

    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error before: %s\n", error_ptr);
        }
    }else{

    }
    printf("%zu\n", strlen(json_string));

    strcpy(t_system_handle->system_name, cJSON_GetObjectItem(json, "system_name")->valuestring);
    t_system_handle->num_sensors = cJSON_GetObjectItem(json, "num_sensors")->valueint;
    j_zones = cJSON_GetObjectItem(json, "zones");
    t_system_handle->zones = (thermo_data_t*)realloc(t_system_handle->zones, sizeof(thermo_data_t)*t_system_handle->num_sensors);
    
    int zone_index = 0;
    cJSON_ArrayForEach(j_zone, j_zones){
        thermo_data_t zone = t_system_handle->zones[zone_index];
        
        strcpy(zone.node_name, cJSON_GetObjectItem(j_zone, "node_name")->valuestring);
        zone.target_temp = cJSON_GetObjectItem(j_zone, "target_temp")->valuedouble;
        zone.measured_temp = cJSON_GetObjectItem(j_zone, "measured_temp")->valuedouble;

        zone_index++;
    }

    cJSON_Delete(json);
}

char* data_export(thermo_sys_t* t_system_handle){

    return "";
}