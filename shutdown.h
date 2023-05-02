//shutdown the program
#ifndef shutdown_h
#define shutdown_h

#include <stdlib.h>
#include <stdio.h>
#include "accelerometer.h"
#include "audioMixer_template.h"
#include "beat.h"
#include "joystickcontrols.h"
#include "terminaloutput.h"
#include "periodTimer.h"

void shutdownaudio(){
    endwriterthread();
    AudioMixer_cleanup();
    cleanupBeats();
    joystickcleanup();
    accelerometerStop();
    
    Period_cleanup();

}


#endif