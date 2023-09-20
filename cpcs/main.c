#include "include.h"
#include "COM.h"
#include "sensor.h"
#include <libudev.h>


int main(int argc, char* argv[])
{
    int serialPort;
    char *portName = argv[1];
    struct termios *tty;

    while (1) {
        openCOMPort(&serialPort, portName);
        configureCOMPort(tty, serialPort);

        if (serialPort != -1) {
            printf("Device connected to %s\n", portName);
            char* result = executeSensorCommand();
            struct sensor* sensors = getSensorsArray(result);
            free(result);

            char *dataToSend = sensorsToString(sensors);
            sendData(serialPort, dataToSend);
            printf("data was sended to MCU.");
            free(dataToSend);
        } else {
            printf("Device disconnected from %s\n", portName);
        }

        sleep(5);
    }
}
