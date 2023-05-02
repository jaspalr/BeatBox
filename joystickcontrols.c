#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "joystick.h"
#include "beat.h"
#include "audioMixer_template.h"
#define defualtBPM 80
static int tempo;
static int volume;
static bool run;
static int beat = 1;
static pthread_t joystickthread;
static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}
//for the joystick thread 0-1 controls the volume 2-3 sets tempo 4 sets beat
static void* joystick(){
    //bool pressed = false;
    while(run){
        for(int i = 0; i < JOYSTICK_MAX_NUMBER_DIRECTIONS; i++){
            
            if(Joystick_isDirectionPressed(i)){
                switch (i)
                {
                case 0://up case
                    volume = AudioMixer_getVolume();
                    if((volume+ 5) < 100){
                        volume += 5;
                        AudioMixer_setVolume(volume);
                    }
                     sleepForMs(100);

                    break;
                case 1: //down case
                    volume = AudioMixer_getVolume();
                    if((volume - 5) > 0){
                        volume -= 5;
                        AudioMixer_setVolume(volume);
                    }
                     sleepForMs(100);
                    break;
                case 2: //left case
                    tempo = getBPM();
                    if((tempo - 5) > 40){
                        tempo -= 5;
                        setBPM(tempo);
                    }
                     sleepForMs(100);
                     break;
                case 3: //right case
                    tempo = getBPM();
                    if((tempo + 5) < 300){
                        tempo += 5;
                        setBPM(tempo);
                    }
                     sleepForMs(100);
                     break;
                case 4: //pressed
                    //if(!pressed){
                        //pressed = true;
                        beat = getbeat();
                        beat++;
                        if(beat > 2){
                            beat = 0;
                        }
                        setbeat(beat);
                         sleepForMs(100);
                     break;
                default:
                    break;
                }
            }
        }
        sleepForMs(100);
    }
    return NULL;

}
//start the joystick thread
void joystickstart(){
    tempo = defualtBPM;
    volume = AudioMixer_getVolume();
    run = true;
    Joystick_init();
    pthread_create(&joystickthread, NULL, joystick, NULL);
}
//ends the joystick thread
void joystickcleanup(){
    run = false;
    pthread_join(joystickthread,NULL);
   
}

