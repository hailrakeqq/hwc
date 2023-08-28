#include "include.h"
#include "COM.h"
#include "sensor.h"

int main()
{
    int serialPort;
    char *portName = "/dev/ttyUSB0";
    char *dataToSend = "";
    struct termios *tty;
    char todo;

    char* result = executeSensorCommand();
    struct sensor* sensors = getSensorsArray(result);
    printSensors(sensors);
    free(sensors);

//    openCOMPort(&serialPort, portName);
//    configureCOMPort(tty, serialPort);

//    while(1){
//        printf("enter what you want to do (x - exit; q - on/off green led; e - on/off red led): ");
//        scanf(" %c", &todo);
//
//        switch (todo) {
//            case 'x':
//                closeConnection(serialPort);
//                free(tty);
//                exit(0);
//            case 'q':
//                dataToSend = "Q";
//                sendData(serialPort, dataToSend);
//                break;
//            case 'e':
//                dataToSend = "E";
//                sendData(serialPort, dataToSend);
//                break;
//        }
//        while (getchar() != '\n');
//    }
}
