#define ISDEBUG

const byte RATE_SIZE = 5;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;

float beatsPerMinute;
int beatAvg;