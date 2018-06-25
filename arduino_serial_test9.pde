import processing.serial.*;
import processing.sound.*;

Serial myPort;
AudioIn input, in;
Amplitude analyzer;
FFT fft;

//approx moving average .5 second noise level
float vol, avgVol = .01, miniVolAvg = .01;
int bands = 32*4, bassScale = 8, midScale = 30, trebScale = 50;
float[] spectrum = new float[bands];
float bass, mid, treb;
int bassAvg = 25, midAvg = 25, trebAvg = 25;


void setup() {
  size(128, 360);
  background(255);
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  println(portName);
  fft = new FFT(this, bands);
  input = new AudioIn(this, 0);
  in = new AudioIn(this, 0);
  
  input.start();
  in.start();
  
  analyzer = new Amplitude(this);
  
  analyzer.input(input);
  fft.input(in);
}


void draw() {
  vol = analyzer.analyze();
  background(255);
  fft.analyze(spectrum);
  avgVol = (avgVol * 1799 + vol) / 1800;
  miniVolAvg = (miniVolAvg * 4 + vol) / 5;
  
  //text(floor(midAvg),20,30);
  
  myPort.write(char(min(floor((miniVolAvg / avgVol) * 20), 31)));
  //myPort.write(char(31));
  delay(1);
  //myPort.write(char(64));
  //delay(1);
  
  for(int i = 0; i < bands; i++) {
    line(i, height, i , height - spectrum[i]*height*15);
  }
  
  bass = 0;
  for(int i = 1; i <= floor(bands/2 / 3); i++) {
    bass += spectrum[i - 1] / miniVolAvg;
  }
  bass = bass * bassScale / floor(bands / 2 / 3) * 32;
  bassAvg = (bassAvg * 49 + floor(bass)) / 50;
  myPort.write(char(min(bassAvg + 32, 63)));
  if(bassAvg < 5) {
    bassScale++;
  }
  if(bassAvg > 45) {
    bassScale--;
  }
  delay(1);

  mid = 0;
  for(int i = floor(bands/2 / 3) + 1; i <= floor(bands/2 / 3) * 2; i++) {
    mid += spectrum[i - 1] / miniVolAvg;
  }
  mid = mid * midScale / floor(bands / 2 / 3) * 32;
  midAvg = (midAvg * 29 + floor(mid)) / 30;
  myPort.write(char(min(midAvg + 64, 95)));
  if(midAvg < 5) {
    midScale++;
  }
  if(midAvg > 45) {
    midScale--;
  }
  delay(1);
  
  treb = 0;
  for(int i = floor(bands/2 / 3) * 2 + 1; i <= bands/2; i++) {
    treb += spectrum[i - 1] / miniVolAvg;
  }
  treb = treb * trebScale / floor(bands / 2 / 3) * 32;
  trebAvg = (trebAvg * 19 + floor(treb)) / 20;
  myPort.write(char(min(trebAvg + 96, 127)));
  if(trebAvg < 5) {
    trebScale++;
  }
  if(trebAvg > 45) {
    trebScale--;
  }
  delay(1);
  
  
}
