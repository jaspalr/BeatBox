#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "udpserver.h"
#include <pthread.h>
#include "beat.h"
#include "audioMixer_template.h"
#include "shutdown.h"

#define MAX_LEN 1500
#define PORT 12345
static unsigned int sin_len;
static struct sockaddr_in sinRemote;
static int socketDescriptor;
static pthread_t udpserver;


static bool command(char* messageRx){
    char messageTx[MAX_LEN] = "\0";
    bool ret = true;


    if(messageRx[0] == 'b' && messageRx[1] == 'e' &&  messageRx[2] == 'a' &&  messageRx[3] == 't' ){//set beat
        setbeat((messageRx[4]) - '0');

    }
    else if(messageRx[0] == 'v' && messageRx[1] == 'o' &&  messageRx[2] == 'l'){//set volume
        char mess[4];
        mess[0] = messageRx[3];
        mess[1] = messageRx[4];
        mess[2] = messageRx[5];
        mess[3] = 0;
        AudioMixer_setVolume(atoi(mess));

    }
    else if(messageRx[0] == 't' && messageRx[1] == 'e' &&  messageRx[2] == 'm'){//set tempo
        char mess[4];
        mess[0] = messageRx[3];
        mess[1] = messageRx[4];
        mess[2] = messageRx[5];
        mess[3] = 0;
        setBPM(atoi(mess));

    }

    else if(strcmp("getbeat",messageRx) == 0){
        snprintf(messageTx, MAX_LEN, "%d",getbeat());
    }

    else if(strcmp("getvol",messageRx) == 0){
        snprintf(messageTx, MAX_LEN, "%d",AudioMixer_getVolume());
    }

    else if(strcmp("getbpm",messageRx) == 0){
        snprintf(messageTx, MAX_LEN, "%d",getBPM());
    }


    else if(strcmp("stop\n",messageRx) == 0){ //case stop
        shutdownaudio();
        ret = false;
    }
    else if(strcmp("Hi-Hat",messageRx) == 0){
        HiHat();
    }
    else if(strcmp("Snare",messageRx) == 0){
        Snare();
    }
    else if(strcmp("Base",messageRx) == 0){
        Base();
    }
    else{ //default
    
        snprintf(messageTx, MAX_LEN, "Help me now!\n");
    }
        
        sin_len = sizeof(sinRemote);
        sendto( socketDescriptor,
        messageTx, strlen(messageTx),
        0,
        (struct sockaddr *) &sinRemote, sin_len);
        
        return ret;
}


static void* serverthread(){
    struct sockaddr_in sin; 
    memset(&sin, 0, sizeof(sin)); 
    sin.sin_family = AF_INET; 
    sin.sin_addr.s_addr = htonl(INADDR_ANY); 
    sin.sin_port = htons(PORT);
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0); 
    bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
    char prev[1024];
    bool run = true;
    sin_len = sizeof(sinRemote);
    char messageRx[MAX_LEN];
    while(run){
        int bytesRx = recvfrom(socketDescriptor,
        messageRx, MAX_LEN - 1, 0,
        (struct sockaddr *) &sinRemote, &sin_len);
        messageRx[bytesRx] = 0;
        if(messageRx[0] =='\n'){
           run = command(prev);
        }
        else{
            run = command(messageRx);
            strcpy(prev,messageRx);
        }
        
    }
    close(socketDescriptor);
    return NULL;
}


void server(){
    pthread_create(&udpserver,NULL,serverthread,NULL);
    pthread_join(udpserver,NULL);
}