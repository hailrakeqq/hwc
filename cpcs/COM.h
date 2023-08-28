#include "include.h"

void openCOMPort(int *serialPort, char *portName);
void configureCOMPort(struct termios *tty, int serialPort);
void sendData(int serialPort, char *data);
void closeConnection(int serialPort);