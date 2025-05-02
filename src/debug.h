#pragma once 

#define ISDEBUG

class
Debug 
{
    public:
        Debug ();

        void setupMAX30205 (uint8_t* address);
        void setupMAX30105 ();
        float getTemperature_MAX30205();
        void setupBPM ();
        float getBPM ();
};

const byte RATE_SIZE = 5;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;

float beatsPerMinute;
int beatAvg;