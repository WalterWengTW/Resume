#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 設定 LCD I2C 位址
// Set the pins on the I2C chip used for LCD connections:
// addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
int V_probe=1;
int C_probe=2;
const int Nmean=3000;
int S=0;
int S_previous=0;
float Vmean_previous=0;
const int Buzzer=9;

void setup() {
  Initializing();
}
void loop() {
  float Vmean=0;
  float Vt=0;
  int C_detect= analogRead(C_probe);
  float Vcal=((C_detect/1024.0)-0.5)*2.0; //Calibration -1 to +1
  
  for(int i=0;i<Nmean;i++)
  {
    //V_detect= analogRead(V_probe);
    Vt=(5-Vcal)*analogRead(V_probe)/1024.0;
    Vmean=Vmean+Vt;
    }
  Vmean=Vmean/Nmean; // Mean of voltage
  if(Vmean!=Vmean_previous)
  {
    lcd.setCursor(10, 0);
    lcd.print(Vmean,2);
    lcd.setCursor(15, 0);
    lcd.print("V");
  }

  if(abs(Vmean-Vmean_previous)<0.2)
  {
    Regonization(Vmean);
    StatusDisplay();
  }
  Vmean_previous=Vmean;
  delay(200);
}
void Initializing(){
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  Serial.begin(9600);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UV Watt Voltage");
  lcd.setCursor(0, 1);
  lcd.print("Measuring Board");
  delay(2000);
  lcd.clear(); 
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print("Wat Volt:");
  lcd.setCursor(0,1);
  lcd.print("Shutter Off");
  pinMode(Buzzer,OUTPUT);
}
void Regonization(float V){
  if(V>1.45 && V<1.90) S=1;
  else if(V>=1.90 && V<=2.5) S=2;
  else if(V>2.5) S=3;
  else S=0;
}
void StatusDisplay(){
  if(S!=S_previous)
  {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    switch(S)
    {
      case 1:
      case 3:
      lcd.setCursor(0,1);
      lcd.print("Out Spec");
      tone(Buzzer,1000);
      break;
      case 2:
      lcd.setCursor(0,1);
      lcd.print("Volt Normal");
      noTone(Buzzer);
      break;
      case 0:
      lcd.setCursor(0,1);
      lcd.print("Shutter Off");
      noTone(Buzzer);
      break;
      default:
      lcd.setCursor(0,1);
      lcd.print("Shutter Off");
      noTone(Buzzer);
    }
    S_previous=S;
  }
}




