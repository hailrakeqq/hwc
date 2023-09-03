#include "include.h"
#include "sensor.h"

void openCOMPort(int *serialPort, char *portName);
void configureCOMPort(struct termios *tty, int serialPort);
void sendData(int serialPort, char *data);
void sendSensorData(int serialPort, struct sensor *sensors);
void closeConnection(int serialPort);