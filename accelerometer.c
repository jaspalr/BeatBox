#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#define config1 "config-pin P9_17 i2c"
#define config2 "config-pin P9_18 i2c"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
static int i2cFileDesc;

static void runCommand(char* command){
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
    // printf("--> %s", buffer); // Uncomment for debugging
        }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

static int initI2cBus(char* bus, int address)
{
    int i2cFile = open(bus, O_RDWR);
    int result = ioctl(i2cFile, I2C_SLAVE, address);
    if (result < 0) {
    perror("I2C: Unable to set I2C device to slave address.");
    exit(1);
    }
    return i2cFile;
}

//starts the accelerometer
void accelerometerInit(){
    runCommand(config1);
	runCommand(config2);
    runCommand("i2cset -y 1 0x1C 0x2A 0x01");
    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, 0x1c);
}

//gets an array of size three and return x value in index 0 y value in index 1, z value in index 2
void accelermeterGetValue(short* array){
    char value[7] = "\0";
    int res = read(i2cFileDesc, &value, 7);
    if (res != sizeof(value)) {
        perror("I2C: Unable to read from i2c register");
        exit(1);
    }
    array[0] = ((short)((value[1]) << 8 | (value[2])))/10000;
    array[1] = ((short)((value[3]) << 8 | (value[4])))/10000;
    array[2] = ((short)((value[5]) << 8 | (value[6])))/10000;
}

//stops the accerometer into stayby 
void accelerometerStop(){
    runCommand("i2cset -y 1 0x1C 0x2A 0x00");
    close(i2cFileDesc);

}