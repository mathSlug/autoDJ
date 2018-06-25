    int REDPIN = 11;
    int GREENPIN = 10;
    int BLUEPIN = 9;
    int XPIN = 0;
    int YPIN = 1;
    int CLICKPIN = 2;
    int r, g, b, clickpos;
    float xpos, ypos;
    char t;
     
     
     
    void setup() {
      pinMode(REDPIN, OUTPUT);
      pinMode(GREENPIN, OUTPUT);
      pinMode(BLUEPIN, OUTPUT);
      pinMode(CLICKPIN, INPUT);
      digitalWrite(CLICKPIN, HIGH);
      r = 0;
      t = '1';
      Serial.begin(9600);
    }
     
     
    void loop() {
      
     clickpos = digitalRead(CLICKPIN);
     xpos = analogRead(XPIN);
     ypos = analogRead(YPIN);
     
     r = floor(255 * (1 * clickpos + 0));
     
     g = floor((xpos/1023) * 255);
     b = floor((ypos/1023) * 255);
     
     if(Serial.available()) {
       t = Serial.read();
     }
     
     /*if(int(t) == 0) {
       color(0,0,0);
     } else {
       color((r),(g),(b));
     }
     */
     color(r*int(t),g*int(t),b*int(t));
     delay(5);
     
      
    }
     
    
    void color(int red, int green, int blue) {
      analogWrite(REDPIN, red);
      analogWrite(GREENPIN, green);
      analogWrite(BLUEPIN, blue);
    }
