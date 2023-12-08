#include "cJSON.h"

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

void data_ingest(char* json_string){
    
}

char* data_export(){
    return "";
}