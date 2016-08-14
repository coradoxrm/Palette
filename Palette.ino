
#define LED 13
#define BUTTON 3
#define ADA 8
#define C 4
#define M 5
#define Y 6
#define K 7

#define printing 13


const unsigned long base = 200000;

String come = "";
String data = "";
int cmyk[4] = {30, 30, 30, 30};

int state;//状态机

const int motor[4] = {C, M, Y, K};

void setup()
{
  Serial1.begin(9600);
  while (!Serial1) ;
  come = "";
  state = 0;

  for (int i = 0 ; i < 4 ; i++) {
    pinMode(motor[i], OUTPUT);
    digitalWrite(motor[i], LOW);
  }
  digitalWrite(printing, LOW);
  //interrupt
  attachInterrupt(digitalPinToInterrupt(BUTTON), print_ISR, RISING);

}



void divide() {
  int count = 0 ;
  String tmp = "";
  //Serial.println(data.length());
  for (int i = 0 ; i < data.length() ; i++)
  {
    if (data[i] == ',') {
      //Serial.print(tmp.toInt());
      cmyk[count] = tmp.toInt();
      count++;
      tmp = "";
      continue;
    }
    tmp += data[i];
  }
  cmyk[count] = tmp.toInt();
  for (int i = 0 ; i < 4 ; i++)
  {
    Serial.println(cmyk[i]);
  }
}

bool check() {
  if (come[come.length() - 1] == '\n')
    return true;
  else
    return false;
}



void output(int portion, int total, int pin) {
  digitalWrite(pin, HIGH);
  unsigned long exe = ((unsigned long)portion) * base / ((unsigned long)total);
  Serial.print("delay:");
  Serial.println(exe);
  delay(exe);
  digitalWrite(pin, LOW);
  delay(1000);
}

void print_ISR() {
  Serial.println("blink");

  if (1) {
  //if (data.length() > 0) {
    digitalWrite(printing, HIGH);
    int total = 0;
    for (int i = 0 ; i < 4 ; i++) {
      total += cmyk[i];
    }

    for (int i = 0 ; i < 4 ; i++)
    {
      Serial.println(cmyk[i]);
      delay(1000);
      output(cmyk[i], total, motor[i]);
    }
  }

  delay(1000);
  digitalWrite(printing, LOW);
  //digitalWrite(test, LOW);
}

void loop()
{
  switch (state) {
    case 0:
      if (Serial1.available())
      {
        char in = (char)Serial1.read();
        come += in;
      }
      if (check()) {
        state = 1;
        Serial.print(come);
      }
      break;

    case 1:
      data = come;
      Serial.print("data= " + data);
      come = "";
      divide();
      delay(200);
      state = 0;
      break;
  }
}
