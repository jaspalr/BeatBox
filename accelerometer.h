//starts/stop the accelerometer and gives the output of the accelermeter
#ifndef accelerometer_h
#define accelerometer_h


//starts the accelerometer
void accelerometerInit();


//gets an array of size three and return x value in index 0 y value in index 1, z value in index 2
void accelermeterGetValue(short* array);

//stops the accerometer into stayby 
void accelerometerStop();

#endif