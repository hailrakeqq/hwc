#ifndef CPCS_SENSOR_H
#define CPCS_SENSOR_H

#include "include.h"
#include <stdint.h>
#include <json-c/json.h>

struct sensor{
    char* name[50];
    float temp;
    uint16_t fanSpeed;
    float ppt;
};

int getSensorCount(char* sensorsCommandResult);
struct sensor* initSensor(int sensorCount);
char* executeSensorCommand();
struct sensor* getSensorsArray(char* input);
void printSensors(struct sensor sensors[]);
int length(struct sensor* sensors);

#endif //CPCS_SENSOR_H
