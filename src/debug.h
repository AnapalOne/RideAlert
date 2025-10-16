/*
  Ride-Alert v0.0.3 (2025-09-17)

  Debug functions for Ride-Alert sensors.
  Currently only supports DHT22 and MAX30102 sensors, 
     with heart rate measurements for the latter only.
  Licenses are stipulated in RideAlert.ino.

  @Andrei Jose R. Embarque <andreijosee@zoho.com>
*/

#pragma once 
#define ISDEBUG

#include <Wire.h>

#include "MAX30105.h"
#include "heartRate.h"
#include "DHT.h"

typedef unsigned char byte;

class Debug 
{
    public:
        Debug ();
        
        static const byte RATE_SIZE = 4;
        byte rates[RATE_SIZE];
        byte rateSpot = 0;
        long lastBeat = 0;

        float beatsPerMinute;
        int beatAvg;

        void setupDHT22 ();
        void setupMAX30105 ();
        void setupBPM ();
        void getDHT22Values ();
        void getBPM ();

    private:
        MAX30105 max30105;
        DHT dht;
};
