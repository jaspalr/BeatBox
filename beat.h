//decides on what beat to play and the BPM
#ifndef beat_h
#define beat_h

//starts the beatplayer
void initBeats();

//set what beat to play
void setbeat(int i);

//sets the BPM
void setBPM(int BPM);

//cleans up beatplayer 
void cleanupBeats();

int getBPM();
int getbeat();

void HiHat();
void Snare();
void Base();
#endif

