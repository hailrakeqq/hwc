#include "COM.h"

void openCOMPort(int *serialPort, char* portName){
    *serialPort = open(portName, O_WRONLY);
    if(*serialPort < 0){
        perror("An error occurred while opening COM-port");
        return;
    }
}

void configureCOMPort(struct termios *tty, int serialPort) {
    tty = (struct termios *)malloc(sizeof(struct termios));

    if(tty == NULL)
    {
        perror("Memory allocation failed");
        free(tty);
    }

    if (tcgetattr(serialPort, tty) != 0)
    {
        perror("An error occurred while configuring the COM-port");
        free(tty);
        exit(1);
    }

    memset(tty, 0, sizeof(struct termios));
    cfsetospeed(tty, B115200);
    cfsetispeed(tty, B115200);

    tty->c_cflag |= (CLOCAL | CREAD);
}

void sendData(int serialPort, char *data){
    write(serialPort, data, strlen(data));
}

void sendSensorData(int serialPort, struct sensor *sensors){
    write(serialPort, sensors, strlen(sensors));
}

void closeConnection(int serialPort){
    close(serialPort);
}