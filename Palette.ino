#include "Adafruit_NeoPixel.h"
#define DI 6
#define LED 13
#define BUTTON 3
#define ADA 8
#define C 4
#define M 5
#define Y 6
#define K 7


const unsigned long base = 3000;

String come = "";
String data = "";
int rgb[3] = {0,0,0};
int cmyk[4] = {0,0,0,0};

int state;//状态机

const int motor[4] = {4,5,6,7};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(25, ADA, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial1.begin(9600);
  while (!Serial1) ;
  come = "";
  state = 0;
  //pixels.begin(); // This initializes the NeoPixel library.
  
//  pinMode(C, OUTPUT);
//  pinMode(M, OUTPUT);
//  pinMode(Y, OUTPUT);
//  pinMode(K, OUTPUT);
//   
//  digitalWrite(C, LOW);
//  digitalWrite(M, LOW);
//  digitalWrite(Y, LOW);
//  digitalWrite(K, LOW);

  for(int i = 0 ;i < 4 ;i++) {
    pinMode(motor[i], OUTPUT);
    digitalWrite(motor[i], LOW);
  }

  //interrupt
  attachInterrupt(digitalPinToInterrupt(BUTTON), print_ISR, RISING);
  
}



void divide() {
  int count = 0 ;
  String tmp = "";
  for(int i = 0 ; i < data.length() ; i++)
  {
    if(data[i] == ','){
      //Serial.print(tmp.toInt());
      rgb[count] = tmp.toInt();
      count++;
      tmp = "";
      continue;
    }
    tmp += data[i];
    
  }
  
  rgb[count] =  tmp.toInt();
  for(int i = 0 ; i < 3 ;i++)
  {
    Serial.println(rgb[i]);
  }
}

bool check() {
  if(come[come.length()-1] == '\n')
    return true;
  else
    return false;
}

void show() {
  Serial.println("in show");
    for(int i=0;i<25;i++){
    pixels.setPixelColor(i, pixels.Color(rgb[0],rgb[1],rgb[2])); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}

void translate() {
    int r = (rgb[0]*100)/255;
    int g = (rgb[1]*100)/255;
    int b = (rgb[2]*100)/255;
    int _max = max(max(r,g),b);
    cmyk[3] = 100 -_max;
    cmyk[0] = (_max-r)*100/_max;
    cmyk[1] = (_max-g)*100/_max;
    cmyk[2] = (_max-b)*100/_max; 
}

void output(int portion, int pin) {
  digitalWrite(pin, HIGH);
  unsigned long exe = ((unsigned long)portion)*base;
  delay(exe);
  digitalWrite(pin, LOW);
  delay(1000);
}

void print_ISR() {
  Serial.println("blink"); 
  
  if(data.length() >0) {
     translate();
     Serial.println("test cmyk");
      for(int i = 0 ; i < 4 ; i++)
      {
        Serial.println(cmyk[i]);
        //output(cmyk[i],motor[i]);
      }
     
      for(int i = 0 ; i < 4 ; i++)
      {
        //Serial.println(cmyk[i]);
        output(cmyk[i],motor[i]);
      }
      delay(1000);
  } 
  else {
    delay(1000);
  }
}

void loop()
{
  switch(state) {
    case 0:
      if(Serial1.available())
      {
        char in = (char)Serial1.read();
        come += in;
      }
      if(check()) {
        state = 1;
        Serial.print(come);
      }
    break;
    
    case 1:
      data = come;
      Serial.print("data= "+data);
      come = "";
      divide();
      show();
      delay(200);
      state = 0;
      break;
  }
}
