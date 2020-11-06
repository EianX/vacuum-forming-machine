#include <LiquidCrystal.h>
#include <max6675.h>

const int encoder[3] = {,,};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int lamp[3] = { , , };
int matrial = 0;
int thick = 0;
const float tempRange[2][2] = {{140.0,190.0},{150.0,180.0}};
const int thickTime[2][6]={{0,30,60,90,120,45},{0,50,100,150,200,75}};
float temp =0.0;
float tMax=0.0,tMin=0.0;
int heatTime=0;
float thick=0.0;
const int heatRelay ;
const int irPin[4]={,,,};
const int irRead[4]={,,,};
const int thermoCouple[3]={,,};
const int buzzer;
const int limitSwitch[3]={,,};
const int fan;
const int vacuum ;
int heatTimeCount=0; 
int sheetCount=0;
int vacuumTime=0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20, 4);
}

void loop() {
  // put your main code here, to run repeatedly:

}
