#include "sensor.h"
#include <ctype.h>

char* executeSensorCommand() {
    FILE *fp;
    char buffer[1024];
    char *command = "sensors -j";
    char *result = NULL;
    size_t result_size = 0;

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error execution command");
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t line_len = strlen(buffer);
        result = realloc(result, result_size + line_len + 1);
        if (result == NULL) {
            perror("Error allocate memory");
            pclose(fp);
            return NULL;
        }

        strcpy(result + result_size, buffer);
        result_size += line_len;
    }

    pclose(fp);
    return result;
}

int getSensorArrayLength(struct sensor* sensors) {
    int length = 0;
    while (sensors[length].name[0] != '\0')
        length++;

    return length;
}

int getSensorCount(char* sensorsCommandResult){
    int sensorCount = 0;
    uint8_t inSensor = 0;

    char* lines[100];
    int lineCount = 0;
    char* line = strtok(sensorsCommandResult, "\n");
    while (line != NULL && lineCount < 100) {
        lines[lineCount++] = line;
        line = strtok(NULL, "\n");
    }

    for (int i = 0; i < lineCount; i++) {
        const char *currentLine = lines[i];

        if (strstr(currentLine, "Adapter:") != NULL || strstr(currentLine, "ERROR:") != NULL) {
            inSensor = 1;
            sensorCount++;
        }

        if (inSensor && isalpha(*currentLine)) {
            inSensor = 0;
        }
    }

    return sensorCount;
}

struct sensor* initSensor(int sensorCount){
    struct sensor* sensors = (struct sensor*) malloc(sensorCount * sizeof(struct sensor));

    for(uint8_t i = 0; i < sensorCount; i++){
        strcpy(sensors[i].name, "");
        sensors[i].fanSpeed = 0;
        sensors[i].ppt = 0;
        sensors[i].temp = 0;
    }

    return sensors;
}

struct sensor* getSensorsArray(char* input) {
    struct json_object *root = json_tokener_parse(input);

    if (root == NULL) {
        printf("Error while parse JSON\n");
        return NULL;
    }

    struct sensor *sensors = (struct sensor*)malloc(2 * sizeof(struct sensor));
    if (sensors == NULL) {
        printf("Error allocate memory\n");
        return sensors;
    }

#pragma region nvme
    struct sensor nvme;
    struct json_object *nvmeObject;
    if (json_object_object_get_ex(root, "nvme-pci-0800", &nvmeObject)) {
        strcpy(nvme.name, "nvme-pci-0800");
        struct json_object *compositeObject;
        if (json_object_object_get_ex(nvmeObject, "Composite", &compositeObject)) {
            struct json_object *temp1InputObject;
            if (json_object_object_get_ex(compositeObject, "temp1_input", &temp1InputObject)) {
                double temp1InputValue = json_object_get_double(temp1InputObject);
                nvme.temp = temp1InputValue;
            }
        }
    }
    sensors[0] = nvme;
#pragma endregion

#pragma region gpu
    struct sensor gpu;
    struct json_object *gpuObject;
    if (json_object_object_get_ex(root, "amdgpu-pci-0100", &gpuObject)) {
        strcpy(gpu.name, "amdgpu-pci-0100");
#pragma region fanSpeed
        struct json_object *fan1Object;
        if (json_object_object_get_ex(gpuObject, "fan1", &fan1Object)) {
            struct json_object *fan1InputObject;
            if (json_object_object_get_ex(fan1Object, "fan1_input", &fan1InputObject)) {
                gpu.fanSpeed = json_object_get_int(fan1InputObject);
            }
        }
#pragma endregion
#pragma region temp
        struct json_object *edgeObject;
        if (json_object_object_get_ex(gpuObject, "edge", &edgeObject)) {
            struct json_object *temp1InputObject;
            if (json_object_object_get_ex(edgeObject, "temp1_input", &temp1InputObject)) {
                gpu.temp = json_object_get_double(temp1InputObject);
            }
        }
#pragma endregion
#pragma region PPT
        struct json_object *PPTObject;
        if (json_object_object_get_ex(gpuObject, "PPT", &PPTObject)) {
            struct json_object *power1_averageObject;
            if (json_object_object_get_ex(PPTObject, "power1_average", &power1_averageObject)) {
                gpu.ppt = json_object_get_double(power1_averageObject);
            }
        }
#pragma endregion
    }
    sensors[1] = gpu;
#pragma endregion

    json_object_put(root);

    return sensors;
}

char* sensorsToString(struct sensor* sensors){
    int sensorCount = getSensorArrayLength(sensors);
    char* result = (char*)malloc(1);
    if (result == NULL) {
        perror("An error occurred while allocating memory");
        exit(1);
    }
    result[0] = '\0';

    for (size_t i = 0; i < sensorCount; i++) {
        char temp[100];
        snprintf(temp, sizeof(temp), "%s;%.2f;%d;%.2f\n", sensors[i].name, sensors[i].temp, sensors[i].fanSpeed, sensors[i].ppt);
        result = (char*)realloc(result, strlen(result) + strlen(temp) + 1);
        if (result == NULL) {
            perror("An error occurred while reallocating memory");
            exit(1);
        }
        strcat(result, temp);
    }
    return result;
}

void printSensors(struct sensor sensors[]){
    for (int i = 0; i < 2; i++) {
        printf("Sensor %d:\n", i + 1);
        printf("Name: %s\n", sensors[i].name);
        printf("Temperature: %.1f°C\n", sensors[i].temp);
        printf("Fan Speed: %u RPM\n", sensors[i].fanSpeed);
        printf("PPT: %.2f W\n", sensors[i].ppt);
        printf("\n");
        free(sensors[i].name);
    }
}

