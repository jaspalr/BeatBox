#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "audioMixer_template.h"
#include "joystick.h"
#include "joystickcontrols.h"
#include "beat.h"
#include "periodTimer.h"
#include "accelerometer.h"
#include "terminaloutput.h"
#include "udpserver.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

int main(){
    Period_init();
    AudioMixer_init();
    joystickstart();
    initBeats();
    accelerometerInit();
    startwriterthread();
    server();
}