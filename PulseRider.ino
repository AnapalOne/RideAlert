/*
  Pulse Rider alpha source code. 
  Version v0.0.1 (2025-05-01)

  As of this version, only debugging features have been implemented, and none of the components (MAX30205 and MAX30102) have not been confirmed to be working. This only serves as a draft for future, hopefully working versions of this project.

  ---------------------------------------------------------------------------

  LICENSES
  PulseRider code is provided under the MIT license. 
  ClosedCube MAX30205 library provided under the MIT license.
  SparkFun MAX3010x Pulse and Proximity Sensor Library is provided under the MIT license.

  @Andrei Jose R. Embarque <andreijosee@zoho.com>
*/

#include <Wire.h>

#include "src/debug.h"

#include <MAX30105.h>
#include <heartRate.h>
#include <spo2_algorithm.h>

#include <ClosedCube_MAX30205.h>

MAX30105 MAX30105;
ClosedCube_MAX30205 MAX30205;

class 
Debug {
    public:
      void
      setupMAX30205 (uint8_t* address)
      {
        // Initialize the MAX30205 sensor
        Serial.println("Starting MAX30205 module...");
        MAX30205.begin(address);
        Serial.println("Initialized MAX30205 module.");
      }

      void
      setupMAX30105 ()
      {
        // Initialize the MAX30105 sensor
        if (MAX30105.begin(Wire, I2C_SPEED_FAST) == false) {
            Serial.println("MAX30105 failed to initialize. Please check wiring/power.");
            while (1);
        }
        Serial.println("Started MAX30102. Setting up module...");
        MAX30105.setup();
        Serial.println("Initialized MAX30102 module.");
      }

      /*
      * Get temperature from MAX30205, in Celsius.
      * If readings consistently show 256.00C, sensor is probably malfunctioning.
      */
      float 
      getTemperature_MAX30205()
      {
          return MAX30205.readTemperature();
      }

      /* Get temperature from MAX30102, in Celsius. */
      void
      setupBPM ()
      {
          MAX30105.setPulseAmplitudeRed(0x0A);
          MAX30105.setPulseAmplitudeGreen(0);
      }

      /* Get BPM from MAX30102. */
      float
      getBPM ()
      {
          long irValue = MAX30105.getIR();

          if (checkForBeat(irValue) == true)
          {
            //We sensed a beat!
            long delta = millis() - lastBeat;
            lastBeat = millis();
        
            beatsPerMinute = 60 / (delta / 1000.0);
        
            if (beatsPerMinute < 255 && beatsPerMinute > 20)
            {
              rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
              rateSpot %= RATE_SIZE; //Wrap variable
        
              //Take average of readings
              beatAvg = 0;
              for (byte x = 0 ; x < RATE_SIZE ; x++)
                beatAvg += rates[x];
              beatAvg /= RATE_SIZE;
            }
          }
        
          return (beatsPerMinute);
      }
};

Debug debug;

void 
setup() 
{
  Serial.begin(115200);

  Serial.println("---------------------------");
  Serial.println("PulseRider v0.0.1 alpha build.");
  Serial.println("---------------------------");
  Serial.println();
  
#ifdef ISDEBUG
  Serial.println("NOTE: DEBUGGING FEATURES HAVE ONLY BEEN IMPLEMENTED. FUTURE COMPUTATIONS WILL BE WORKED ON IN FUTURE IMPLEMENTATIONS.");

  debug.setupMAX30205(0x90);
  debug.setupMAX30105();
  debug.setupBPM();
  Serial.println("Place finger on sensor");
#endif

}

void 
loop() 
{
#ifdef ISDEBUG
  Serial.println(debug.getTemperature_MAX30205());
  Serial.println(debug.getBPM());
#endif

  delay(1000);
}
