#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "beat.h"
#include "joystickcontrols.h"
#include "periodTimer.h"
#include "audioMixer_template.h"

static pthread_t writerthread;
static bool run;

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

//thread for writing output to the terminal 
void* writer(){
    Period_statistics_t beats;
    Period_statistics_t acc;
    while(run){
        Period_getStatisticsAndClear(0,&beats);
        Period_getStatisticsAndClear(1,&acc);
        printf("M%d, %dbpm vol:%d",getbeat(),getBPM(),AudioMixer_getVolume());
        printf("Audio[%0.3f, %0.3f] avg %0.3f/%d ",beats.minPeriodInMs, beats.maxPeriodInMs, beats.avgPeriodInMs, beats.numSamples);
        printf("Accel[%0.3f, %0.3f] avg %0.3f/%d\n",acc.minPeriodInMs, acc.maxPeriodInMs, acc.avgPeriodInMs, acc.numSamples);
        sleepForMs(1000);
    }
    return NULL;
}
 

//starts the thread to write to the terimal 
void startwriterthread(){
    run = true;
    pthread_create(&writerthread,NULL, &writer, NULL);


}
//ends that thread 
void endwriterthread(){
    run = false;
    pthread_join(writerthread,NULL);

}