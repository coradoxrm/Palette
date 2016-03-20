#include "Adafruit_NeoPixel.h"
#define DI 6
#define LED 13
#define BUTTON 9
#define ADA 8

String come = "";
String data = "";
int rgb[3] = {0,0,0};

int state;//状态机

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(25, ADA, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial1.begin(9600);
  while (!Serial1) ;
  come = "";
  state = 0;
  pixels.begin(); // This initializes the NeoPixel library.
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
  //Serial.print(tmp.toInt());
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

      if(data.length() >0 && digitalRead(BUTTON) == HIGH) {
        state = 2;
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

    case 2:
      //TODO pump and traslate RGB to CMYK
      
      
      
      break;
  }
}
