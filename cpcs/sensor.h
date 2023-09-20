#ifndef SENSOR_H
#define SENSOR_H

#include "include.h"
#include <stdint.h>
#include <json-c/json.h>
 struct sensor{
    char* name[25];
    float temp;
    uint16_t fanSpeed;
    float ppt;
};

int getSensorCount(char* sensorsCommandResult);
struct sensor* initSensor(int sensorCount);
char* executeSensorCommand();
struct sensor* getSensorsArray(char* input);
char* sensorsToString(struct sensor* sensors);
int getSensorArrayLength(struct sensor* sensors);

#endif
