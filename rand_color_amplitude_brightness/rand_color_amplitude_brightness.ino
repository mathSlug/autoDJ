/****************************************
Emeep
****************************************/

#define REDPIN 9
#define GREENPIN 10
#define BLUEPIN 11

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

double voltmin = 1.0;
double voltmax = 1.0;

int rmod = 150;
int gmod = 150;
int bmod = 150;
 
void setup() 
{
   Serial.begin(9600);
}
 
 
void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }

         //color(sample/4-2,sample/4-2,sample/4-2);
         //delay(100);
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts

   if (volts < voltmin * 1.05)
    {
      randCol();
      voltmin = volts;
    }
   if (volts * 1.05 > voltmax)
    {
      randCol();
      voltmax = volts;
    }
 
   Serial.println(volts);
   volts = volts * volts;
   color(volts*rmod,volts*gmod,volts*bmod);

   voltmax = voltmax * 0.999;
   voltmin = voltmin * 1.0005;
}

void color(int red, int green, int blue) {
      analogWrite(REDPIN, min(red,255));
      analogWrite(GREENPIN, min(green,255));
      analogWrite(BLUEPIN, min(blue,255));
    }

void randCol() {
  rmod = random(20,180);
  gmod = random(20,180);
  bmod = random(20,180);
}
