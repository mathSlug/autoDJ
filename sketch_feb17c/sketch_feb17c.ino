/*
 * hi!
 * JB 2020
 */

#include "arduinoFFT.h"

#define REDPIN 9
#define GREENPIN 10
#define BLUEPIN 11

double oldGreen = 0;
double blue = 0;
double oldBlue = 0;

arduinoFFT FFT = arduinoFFT();

int bigSamps = 400;
const uint16_t samples = 128; //This value MUST ALWAYS be a power of 2

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double vReal[samples];
double vImag[samples];

unsigned int scaleAvg = 1;

void setup() {
  Serial.begin(9600);
  analogReference (EXTERNAL);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
}

void loop() {
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  for (int i = 0; i < bigSamps; i++)
  {
    int thisSamp = analogRead(0);
    if (thisSamp < 1024)  { //throw out spurious
      if (thisSamp > signalMax)  {
        signalMax = thisSamp;  // save just the max levels
      }
      else if (thisSamp < signalMin)  {
        signalMin = thisSamp;  // save just the min levels
      }
    }

    if(i > bigSamps - samples) {
      vReal[i - bigSamps + samples] = int8_t(thisSamp);
      vImag[i - bigSamps + samples] = 0.0;
    }
  }

  unsigned int scale = signalMax - signalMin;

  double red = 0;
  double oldRed = 0;
  double green = 0;

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, samples);
  
  for(int i = 1; i < (samples >> 1) - 1; i++) {
    if(i < (samples >> 1) / 4 - 2) {
      red += vReal[i] * 4;
    }
    else if(i <= (samples >> 1) - (samples >> 1) / 4 - 2) {
      green += vReal[i];
    }
    else if(i < (samples >> 1) - 10) {
      blue += vReal[i] * 3;
    }
  }

  red = (red * red + 4*oldRed)/5;
  green = (green * green + 4*oldGreen)/5;
  blue = (blue * blue + 4*oldBlue)/5;

  oldRed = red;
  oldGreen = green;
  oldBlue = blue;

  unsigned long normFact = sqrt(red*red + green*green + blue*blue);

  red = red / normFact;
  green = green / normFact;
  blue = blue / normFact;

  scale = scale*255 / 1024;

  color(red * scale, green * scale, blue * scale);
}


void color(int r, int g, int b) {
  analogWrite(REDPIN, min(r,255));
  analogWrite(GREENPIN, min(g,255));
  analogWrite(BLUEPIN, min(b,255));
}
