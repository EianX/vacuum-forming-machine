//Including required libraries
#include <LiquidCrystal.h>
#include <max6675.h>
//Pins variables
const int encoder[3] = {,,};
const int lamp[3] = { , , };
const int heatRelay;
const int irPin[4]={,,,};
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
const int buzzer;
const int limitSwitch[3]={,,};
const int fan;
const int vacuum;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//Data variables
int matrial = 0; //Acrylic-PVC:0, PP:1
int thick = 0;
float temp =0.0;
float tempMax=0.0,tempMin=0.0;
int heatTime=0;
float thick=0.0;
int heatTimeCount = 0; 
int sheetCount = 0;
int vacuumTime = 0;
int irRead[4]={,,,};
//LCD Menues variables
int counter = 0;
int menu = 0;
int opt = 0;
int last_counter = 0;
bool clk_State;
bool Last_State; 
bool dt_State;
//Material and thickness heat forming data
const float tempRange[2][2] = {{140.0,190.0},{150.0,180.0}};
const int thickTime[2][6]={{0,30,60,90,120,45},{0,50,100,150,200,75}}; //Index = thick except 1.5mm thickness is index 5
//LCD special characters
uint8_t arrow[8] = {0x0, 0x04 ,0x06, 0x1f, 0x06, 0x04, 0x00, 0x00};

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20, 4);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void heaters() {
  //Jahein part
for(int i=0;i<4,i++){
  irRead[i] = digitalRead(irPin[i]);
}
if(irRead[0]==1 && irRead[1]==1 && irRead[2]==1 && irRead[3]==1){
  lcd.clear();
  lcd.print("Pull the heater");
}
else{
  lcd.clear();
  lcd.print("Put the sheet");
}
int limitStart = digitalRead(limitSwitch[0];
int limitEnd = digitalRead(limitSwitch[1];
if(limitStart == 1){
  digitalWrite(heatRelay , HIGH);
}
else{
  lcd.clear();
  lcd.print("Pull the Heater");
}
temp = thermocouple.readCelsius();
if (t<tf){
  digitalWrite(heatRelay , HIGH);
  digitalWrite(lamp[1] , HIGH);
  if(temp>tempMax){
    digitalWrite(heatRelay , LOW);
  }
  if(temp<tempMin){
    digitalWrite(heatRelay , HIGH);
  }
}
else {
  digitalWrite(heatRelay , LOW);
  digitalWrite(lamp[1] , LOW);
  lcd.clear();
  lcd.print("Push heaters");
  digitalWrite(buzzer,HIGH);
}
if(limitEnd==1){
  digitalWrite(buzzer,HIGH);
}
else{
  lcd.print("Push heaters");
  digitalWrite(buzzer,HIGH);
}








void vacuum() {
  //Osman part
lcd.print(0,0) "Pull up Symbol";
digitalRead limitSwitch[];
if (limitSwitch(,HIGH)){
  digitalWrite(vacuum,High);
 for(int i=0 ; i<18 ; i++){
  last_counter=18 - i;
  lcd.print(0,0) "Countdown : (last_counter) ";
  delay(1000);
  do{digitalWrite(fan,HIGH);}
  while {last_counter==8;}
  do{digitalWrite(vacuum,LOW);}
  while {last_counter==5;}
  do{digitalWrite(fan,LOW);}
  while {last_counter==0;}
 }
 lcd.print(0,0) "Sheet (sheetCount) is ready..";
 sheetCount=sheetCount++;
 lcd.print(3,0) "Counter (counter)";
 counter=counter++;
 lcd.print(4,0) "Repeat ..?";
 digitalRead(encoder[]);
 if (encoder[],HIGH){
  goto 
  }
  else{
    digitalWrite(selector,LOW);
    lcd.print (0,0) "Sleeping..";
    }
 }
 
else { lcd.print(0,0) "Put the sheet.."
}
}
