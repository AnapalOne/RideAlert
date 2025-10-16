/*
  Ride-Alert alpha source code. 
  Version v0.0.3 (2025-09-17)

  As of this version, only debugging features have been implemented, but both 
    components (DHT22 and MAX30102) have been tested and working.
  Functionality to bring sensor values to LCD screen will come in later versions.
  Additionally, MAX30205 has been switched to DHT22, due to sourcing issues.

  Ride-Alert code is provided under the MIT license. 
  SparkFun MAX3010x Pulse and Proximity Sensor Library is provided under 
    the MIT license.
  DHT sensor library is provided under the MIT license.

  @Andrei Jose R. Embarque <andreijosee@zoho.com>
*/

#include <Wire.h>

#include "MAX30105.h"
#include "heartRate.h"
#include "DHT.h"

#include "src/debug.h"
Debug debug;

void 
setup() 
{
  Serial.begin(115200);

  Serial.println("---------------------------");
  Serial.println("Ride-Alert v0.0.3 alpha build.");
  Serial.println("---------------------------");
  Serial.println();
  
  Wire.begin();
  
#ifdef ISDEBUG
  Serial.println("NOTE: DEBUGGING FEATURES HAVE ONLY BEEN IMPLEMENTED. FUTURE COMPUTATIONS WILL BE WORKED ON IN FUTURE IMPLEMENTATIONS.");
  Serial.println();
  delay(5000);

  Serial.print("Initializing sensors...");
  debug.setupDHT22();
  debug.setupMAX30105();
  debug.setupBPM();
  Serial.println(" done.");
#endif

}

void 
loop() 
{
  unsigned long currentTime = millis();
  
  static unsigned long dhtPrev = 0;
  const unsigned int dhtDelay = 1000; 

#ifdef ISDEBUG
  if (currentTime - dhtPrev >= dhtDelay) {
    Serial.print("DEBUG: DHT22: ");
    debug.getDHT22Values();
    dhtPrev = currentTime;
  }

  Serial.print("DEBUG: MAX30102: ");
  debug.getBPM();
#endif
}
