//Including required libraries
#include <LiquidCrystal.h>
#include <max6675.h>
//Pins variables
const int encoder[3] = {,,};
const int lamp[3] = { , , };
const int heatRelay;
const int irPin[4]={,,,};
const int thermoCouple[3]={,,};
const int buzzer;
const int limitSwitch[3]={,,};
const int fan;
const int vacuum;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//Data variables
int matrial = 0; //Acrylic-PVC:0, PP:1
int thick = 0;
float temp =0.0;
float tMax=0.0,tMin=0.0;
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


}


void vacuum() {
  //Osman part

  
}
