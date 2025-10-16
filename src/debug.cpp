/*
  Ride-Alert v0.0.3 (2025-09-17)

  Debug functions for Ride-Alert sensors.
  Currently only supports DHT22 and MAX30102 sensors, 
     with heart rate measurements for the latter only.
  Licenses are stipulated in RideAlert.ino.

  @Andrei Jose R. Embarque <andreijosee@zoho.com>
*/

#include <Arduino.h>
#include "debug.h"

#define DHTPIN 2 
#define DHTTYPE DHT22

Debug::Debug () : dht(DHTPIN, DHTTYPE)
{
}

void
Debug::setupDHT22 ()
{
  Serial.print("Starting DHT22...");
  dht.begin();
  Serial.println(" done.");
}

/* note: MAX30102 and MAX30105 can be change interchangably.
//       This project will be using the MAX30102, but will be still
//          using the MAX30105 name nonetheless.
*/
void
Debug::setupMAX30105 ()
{
  Serial.print("Starting MAX30105...");

  if (max30105.begin(Wire, I2C_SPEED_FAST) == false) {
    Serial.println(" failed!. Please check wiring/power.");
    while (1);
  } 
  else
    Serial.println(" done.");

  Serial.print("Initializing MAX30102...");
  max30105.setup();
  Serial.println(" done.");
}

void
Debug::setupBPM ()
{
  max30105.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  max30105.setPulseAmplitudeGreen(0); //Turn off Green LED
}

void
Debug::getDHT22Values ()
{
  do {
    float humidity = dht.readHumidity();
    float tempC    = dht.readTemperature();     // in celcius
    float tempF    = dht.readTemperature(true); // in fahrenheit

    // if values return NaN, aka failed, restart attempt.
    if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      delay(1000);
      continue;
    }

    float heatIndexF = dht.computeHeatIndex(tempF, humidity);
    float heatIndexC = dht.computeHeatIndex(tempC, humidity, false);

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("% | Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ");
    Serial.print(tempF);
    Serial.print("°F | Heat index: ");
    Serial.print(heatIndexC);
    Serial.print("°C ");
    Serial.print(heatIndexF);
    Serial.println("°F");
    break;
  } while (true);
}

void
Debug::getBPM ()
{
 long irValue = max30105.getIR();

  if (checkForBeat(irValue))
  {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; 
      rateSpot %= RATE_SIZE; 

      // compute for average BPM
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(" | BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(" | Avg BPM=");
  Serial.print(beatAvg);

  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();
}
