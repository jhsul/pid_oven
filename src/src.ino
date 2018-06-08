#include <LiquidCrystal.h>
#include <max6675.h>

// LCD pins
const int d7 = 13;
const int d6 = 12;
const int d5 = 11;
const int d4 = 10;
const int en = 9;
const int rs = 8;

// Thermocouple pins
const int ck = 6;
const int cs = 5;
const int dg = 4;

// Control output pin
const int co = 2;

// Potentiometer pin (analog)
const int pot = 1;

// PID constants
const double kp = 0.5f;
const double ki = 0.01f;
const double kd = 0.5f;

// Cycle Time (SECONDS)
const int period = 10;

// Singleton objects
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
MAX6675 thermocouple(ck, cs, dg);

// Setpoint and Process Variable
double sp;
double pv;

// PID elements (pe is just the error)
double ie = 0.0f;
double de;

// Errors
const int ecount = 5; // amount of previous errors stored
double errors[ecount]; // with 0 as the most recent one, 4 as the oldest

void setup()
{
  lcd.begin(16, 2);
  pinMode(co, OUTPUT);
  Serial.begin(9600);
  
  for(int i = 0; i < ecount; errors[i++] = 0.0f);
}

void loop()
{
  //static int cnt = 0;
  pv = thermocouple.readCelsius();
  sp = map(analogRead(pot), 0, 1023, 0, 200);
  
  disp(sp, pv);
  
  // Cycle the error values
  for(int i = ecount - 1; i > 0; --i)
  {
    errors[i] = errors[i-1];
  }
  errors[0] = sp - pv;

  ie = 0.0f;
  for(int i = 0; i < ecount; ++i)
  {
    ie += errors[i];
  }
  ie *= period;
  de = (errors[0] - errors[1])/period;

  double output = kp * errors[0]
                + ki * ie
                + kd * de;

  Serial.print("pv=");
  Serial.println(pv);

  Serial.print("ie=");
  Serial.println(ie);

  Serial.print("de=");
  Serial.println(de);

  Serial.print("output=");
  Serial.println(output);

  Serial.println("");

  for(int i = 0; i < ecount; ++i)
  {
    Serial.print("errors[");
    Serial.print(i);
    Serial.print("]=");
    Serial.println(errors[i]);
  }
  Serial.println("");
  
  heat(output);
}

/*
Takes po on [0, 1.0]
*/
void heat(double output)
{
  if(output > 1.0f)
  {
    digitalWrite(co, HIGH);
    delay(1000 * period);
  }
  else if(output < 0.0f)
  {
    digitalWrite(co, LOW);
    delay(1000 * period);
  }
  else
  {
    int dur = (int) (output * (double) (1000 * period));
    digitalWrite(co, HIGH);
    delay(dur);
    digitalWrite(co, LOW);
    delay((1000 * period) - dur);
  }
}


void disp(double sp, double pv)
{
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("SP: ");
  lcd.print(sp);
  lcd.print(" C");
  
  lcd.setCursor(0, 1);
  lcd.print("PV: ");
  lcd.print(pv);
  lcd.print(" C");
}

