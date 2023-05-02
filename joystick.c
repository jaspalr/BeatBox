//
// Created by brian on 9/15/16.
//

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "joystick.h"
#include <stdbool.h>
#include <string.h>
#include <time.h>
static void assertDirectioOk(enum eJoystickDirections direction);

struct DirectionInfo {
    const char* name;
    const int portNumber;
    const char* FILE;
};

// Data on all different directions which we support
const struct DirectionInfo directions[] = {
        {"Up",     26, "/sys/class/gpio/gpio26/value"},
        {"Down",   46, "/sys/class/gpio/gpio46/value"},
        {"Left",   65,"/sys/class/gpio/gpio65/value"},
        {"Right",  47,"/sys/class/gpio/gpio47/value"},
        {"Center", 27,"/sys/class/gpio/gpio27/value"},
};

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
static bool readFromFileToScreen(const char *fileName){
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }
    // Read string (line)
    const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, pFile);
    // Close
    fclose(pFile);
    if(strcmp(buff, "1\n") == 0){
        return true;
    }
    else{
        return false;
    }
    
}

_Bool Joystick_init()
{
    runCommand("config-pin p8.14 gpio");
    runCommand("config-pin p8.15 gpio");
    runCommand("config-pin p8.16 gpio");
    runCommand("config-pin p8.18 gpio");


    return 1;
}


_Bool Joystick_isDirectionPressed(enum eJoystickDirections direction)
{
    assertDirectioOk(direction);
    return !readFromFileToScreen(directions[direction].FILE);
}

const char* Joystick_getDirectionName(enum eJoystickDirections direction)
{
    assertDirectioOk(direction);

    return directions[direction].name;
}



static void assertDirectioOk(enum eJoystickDirections direction)
{
    assert(direction >= 0 && direction < JOYSTICK_MAX_NUMBER_DIRECTIONS);
}