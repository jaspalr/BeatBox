#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "audioMixer_template.h"
#include <pthread.h>
#include "accelerometer.h"
#include "periodTimer.h"

#define defualtBPM 80
static int sleepBPM;
static int BPMRATE;
static wavedata_t* base;
static wavedata_t* hi;
static wavedata_t* snare;
static wavedata_t* snarehard;
static wavedata_t* drumhard;
static int beat;
static bool beatchange = false;
static pthread_t beatplayer;
static pthread_t accelerometer_manger;
bool run;
static int xprev = 0;
static int yprev = 0;
static int zprev = 1;


static void sleepForBPM(){
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = sleepBPM * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
    
}

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

static void beat1(){
    AudioMixer_queueSound(hi);
    AudioMixer_queueSound(base);
    sleepForBPM();
    AudioMixer_queueSound(hi);
    sleepForBPM();
    AudioMixer_queueSound(hi);
    AudioMixer_queueSound(snare);
    sleepForBPM();
    AudioMixer_queueSound(hi);
    sleepForBPM();
    AudioMixer_queueSound(hi);
    AudioMixer_queueSound(base);
    sleepForBPM();
    AudioMixer_queueSound(hi);
    sleepForBPM();
    AudioMixer_queueSound(hi);
    AudioMixer_queueSound(snare);
    sleepForBPM();
    AudioMixer_queueSound(hi);
    sleepForBPM();
}
static void beat2(){
    AudioMixer_queueSound(snarehard);
    sleepForBPM();
    AudioMixer_queueSound(drumhard);
    AudioMixer_queueSound(hi);
    sleepForBPM();
    AudioMixer_queueSound(snarehard);
    sleepForBPM();
    AudioMixer_queueSound(drumhard);
    AudioMixer_queueSound(snarehard);
    sleepForBPM();
    AudioMixer_queueSound(hi);
    sleepForBPM();
    AudioMixer_queueSound(hi);
    AudioMixer_queueSound(snarehard);
    sleepForBPM();
    AudioMixer_queueSound(snarehard);
    sleepForBPM();
    AudioMixer_queueSound(drumhard);
    AudioMixer_queueSound(hi);
    sleepForBPM();
}

static void* accelerometerfun(){
    short arr[3];
    while(run){
        accelermeterGetValue(arr);
      
    
        Period_markEvent(PERIOD_EVENT_ACC);
        if(arr[0] != xprev && arr[0] != 0){
            xprev = arr[0];
            AudioMixer_queueSound(hi);
        
        }
        if(arr[1] != yprev && arr[1] != 0){

            yprev = arr[1];

            AudioMixer_queueSound(snare);
        }
        if(arr[2] != zprev && arr[2] != 1){

            zprev = arr[2];

            AudioMixer_queueSound(base);

        }
        sleepForMs(25);
 
    }
    return NULL;
}

//cylces between beats and tells audioMixer what to play, 0 is none 1 is rock beat 2 is custom
static void* beatplayerfun(){
    

    while(run){

       
        switch (beat)
        {
            case 0:
                if(beatchange){
                    AudoiMixer_clearQuene();
                    beatchange = false;
                }
                
                break;
            case 1:
            if(beatchange){
                AudoiMixer_clearQuene();
                beatchange = false;
            }
                beat1();
                break;
            case 2:
            if(beatchange){
                AudoiMixer_clearQuene();
                beatchange = false;
            }
                beat2();
                break;
            default:
                break;
            }
        
      
    }
    return NULL;
}

void HiHat(){
    AudioMixer_queueSound(hi);
}

void Snare(){
    AudioMixer_queueSound(snare);
}

void Base(){
    AudioMixer_queueSound(base);
}

//sets the BPM
void setBPM(int BPM){
    BPMRATE = BPM;
    double temp = (60.0/BPM/2.0) * 1000;
    sleepBPM = temp;

}

int getBPM(){
    return BPMRATE;

}

//starts the beatplayer
void initBeats(){
    base = malloc(sizeof(wavedata_t));
    hi = malloc(sizeof(wavedata_t));
    snare = malloc(sizeof(wavedata_t));
    snarehard = malloc(sizeof(wavedata_t));
    drumhard = malloc(sizeof(wavedata_t));
    setBPM(defualtBPM);


    sleepForBPM();
    AudioMixer_readWaveFileIntoMemory("wave-files/100051__menegass__gui-drum-bd-hard.wav",base);
    AudioMixer_readWaveFileIntoMemory("wave-files/100053__menegass__gui-drum-cc.wav",hi);
    AudioMixer_readWaveFileIntoMemory("wave-files/100059__menegass__gui-drum-snare-soft.wav",snare);
    AudioMixer_readWaveFileIntoMemory("wave-files/100058__menegass__gui-drum-snare-hard.wav",snarehard);
    AudioMixer_readWaveFileIntoMemory("wave-files/100056__menegass__gui-drum-cyn-hard.wav",drumhard);
    beat = 1;
    run = true;
    accelerometerInit();
    pthread_create(&accelerometer_manger, NULL, accelerometerfun, NULL);
    pthread_create(&beatplayer, NULL, beatplayerfun, NULL);
}

//set what beat to play
void setbeat(int i){


    beat = i;
    beatchange = true;

}

int getbeat(){
    return beat;
}




//cleans up beatplayer 
void cleanupBeats(){
    run = false;
    pthread_join(accelerometer_manger,NULL);
    pthread_join(beatplayer,NULL);
    accelerometerStop();
    free(base->pData);
    free(hi->pData);
    free(snare->pData);
    free(snarehard->pData);
    free(drumhard->pData);
    free(base);
    free(hi);
    free(snare);
    free(snarehard);
    free(drumhard);
}

