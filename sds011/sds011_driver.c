#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define SDS011_DEVICE "/dev/ttyUSB0"

int main() {
    int fd;
    struct termios tio;

    // Open the SDS011 device file
    fd = open(SDS011_DEVICE, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        printf("Error opening SDS011 device file\n");
        return 1;
    }

    // Configure the serial port
    memset(&tio, 0, sizeof(tio));
    tio.c_cflag = CS8 | CREAD | CLOCAL;
    tio.c_cc[VTIME] = 0;
    tio.c_cc[VMIN] = 0;
    cfsetospeed(&tio, B9600);
    cfsetispeed(&tio, B9600);
    tcsetattr(fd, TCSANOW, &tio);

    // Send command to start continuous measurement
    char command[] = { 0xAA, 0xB4, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x05, 0xAB };
    write(fd, command, sizeof(command));

    // Read data from the sensor
    while (1) {
        char buffer[10];
        int bytes_read = read(fd, buffer, sizeof(buffer));

        if (bytes_read == 10) {
            // Parse the data
            int pm25 = buffer[2] + (buffer[3] << 8);
            int pm10 = buffer[4] + (buffer[5] << 8);
            printf("PM2.5: %d, PM10: %d\n", pm25, pm10);
            fflush(stdout);

        }
    }

    // Close the SDS011 device file
    close(fd);

    return 0;
}
