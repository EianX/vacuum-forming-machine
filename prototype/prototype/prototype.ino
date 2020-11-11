//Including required libraries
#include <LiquidCrystal.h>
#include <max6675.h>
#include <Encoder.h>
//Pins variables
Encoder myEnc(, );
const int encButton = ;
const int lamp[4] = { , , , };
const int heatRelay;
const int irPin[4]={,,,};
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
const int buzzer;
const int limitSwitch[3]={,,};
const int fan;
const int vacuum;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//Data variables
int material = 0;//Acrylic-PVC:0, PP:1
String materialName = "";
int thick = 0;
float temp =0.0;
float tempMax=0.0,tempMin=0.0;
int heatTime=0;
int thick=0;
int heatTimeCount = 0; 
int sheetCount = 0;
int vacuumTime = 0;
int irRead[4]={,,,};
//LCD Menues variables
long counter = 0;
int menu = 0;
int opt = 0;
long lastCounter = -999;
bool pushed = true;
//Material and thickness heat forming data
const float tempRange[2][2] = {{140.0,190.0},{150.0,180.0}};
const int thickTime[2][4]={{0,30,60,90},{0,50,100,150}}; //Index = thickness
//LCD special characters
uint8_t arrow[8] = {0x0, 0x04, 0x06, 0x1f, 0x06, 0x04, 0x00, 0x00};
uint8_t checked[8] = {0x1F, 0x13, 0x13, 0x13, 0x1B, 0x1B, 0x15, 0x1F};


void setup() {
  // put your setup code here, to run once:
  pinMode(encButton, INPUT);
  
  lcd.begin(20, 4);
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight
  lcd.createChar(0, arrow);   //Create the arrow symbol
  lcd.createChar(1, checked);
  lcd.home();                 //Home the LCD

  lcd.clear();
  lcd.setCursor(3, 0);  
  lcd.print("Vacuum Forming");
  lcd.setCursor(2, 1);  
  lcd.print("Machine Prototype");
  lcd.setCursor(5, 3);  
  lcd.print("WELCOME :D");
  delay(2500);
}

void loop() {
  // put your main code here, to run repeatedly:
  counter = myEnc.read();
  
  if(counter > 17){
    counter = 17;
  }
  if(counter < 0){
    counter = 0;
  }

  if(counter != lastCounter || pushed){
    pushed = false;
    
    if(menu == 0)
    {  
      if(0 <= counter && counter < 6)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select material:");
        lcd.setCursor(0,1);
        lcd.write(0);  
        lcd.print("PVC");
        lcd.setCursor(0,2);  
        lcd.print(" Acrylic");
        lcd.setCursor(0,3);  
        lcd.print(" PP");
        opt = 1;
      }
      if(6 <= counter && counter < 12)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select material:");
        lcd.setCursor(0,1);  
        lcd.print(" PVC");
        lcd.setCursor(0,2); 
        lcd.write(0); 
        lcd.print("Acrylic");
        lcd.setCursor(0,3);  
        lcd.print(" PP");
        opt = 2;
      }
      if(12 <= counter && counter < 18)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select material:");
        lcd.setCursor(0,1);  
        lcd.print(" PVC");
        lcd.setCursor(0,2);  
        lcd.print(" Acrylic");
        lcd.setCursor(0,3); 
        lcd.write(0); 
        lcd.print("PP");
        opt = 3;
      }
    }
    if(menu == 1)
    {  
      if(0 <= counter && counter < 6)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select thickness:");
        lcd.setCursor(0,1);
        lcd.write(0);  
        lcd.print("1 mm");
        lcd.setCursor(0,2);  
        lcd.print(" 2 mm");
        lcd.setCursor(0,3);  
        lcd.print(" 3 mm");
        opt = 1;
      }
      if(6 <= counter && counter < 12)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select thickness:");
        lcd.setCursor(0,1);  
        lcd.print(" 1 mm");
        lcd.setCursor(0,2); 
        lcd.write(0); 
        lcd.print("2 mm");
        lcd.setCursor(0,3);  
        lcd.print(" 3 mm");
        opt = 2;
      }
      if(12 <= counter && counter < 18)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select thickness:");
        lcd.setCursor(0,1);  
        lcd.print(" 1 mm");
        lcd.setCursor(0,2);  
        lcd.print(" 2 mm");
        lcd.setCursor(0,3); 
        lcd.write(0); 
        lcd.print("3 mm");
        opt = 3;
      }
    }
    if(menu == 2)
    {  
      if(0 <= counter && counter < 9)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);
        lcd.print("Material: ");
        lcd.print(materialName);
        lcd.setCursor(0,1);
        lcd.write(1);  
        lcd.print("Thickness: ");
        lcd.print(thick);
        lcd.print(" mm");
        lcd.setCursor(0,2); 
        lcd.write(0); 
        lcd.print("Continue");
        lcd.setCursor(0,3);  
        lcd.print(" Return");
        opt = 1;
      }
      if(9 <= counter && counter < 18)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);
        lcd.print("Material: ");
        lcd.print(materialName);
        lcd.setCursor(0,1);
        lcd.write(1);  
        lcd.print("Thickness: ");
        lcd.print(thick);
        lcd.print(" mm");
        lcd.setCursor(0,2);  
        lcd.print(" Continue");
        lcd.setCursor(0,3);
        lcd.write(0);  
        lcd.print("Return");
        opt = 2;
      }
    }
    if(menu == 3)
    {  
      if(0 <= counter && counter < 9)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Total sheet count:");
        lcd.setCursor(0,1);
        lcd.write(1);  
        lcd.print("  ");
        lcd.print(sheetCount);
        lcd.print("  sheets");
        lcd.setCursor(0,2);
        lcd.write(0);  
        lcd.print("Make another sheet");
        lcd.setCursor(0,3);  
        lcd.print(" Return to options");
        opt = 1;
      }
      if(9 <= counter && counter < 18)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Total sheet count:");
        lcd.setCursor(0,1);
        lcd.write(1);  
        lcd.print("  ");
        lcd.print(sheetCount);
        lcd.print("  sheets");
        lcd.setCursor(0,2);  
        lcd.print(" Make another sheet");
        lcd.setCursor(0,3); 
        lcd.write(0); 
        lcd.print("Return to options");
        opt = 2;
      }
    }
  }

  lastCounter = counter;

  if(digitalRead(encButton)){
    pushed = true;
    myEnc.write(0);
  
    if(menu == 0)
    {
      menu = 1;
    
      if(opt == 1)
      {
        material = 0;
        materialName = "PVC";
      }
      if(opt == 2)
      {
        material = 0;
        materialName = "Acrylic";
      }
      if(opt == 3)
      {
        material = 1;
        materialName = "PP";
      }
    }
    if(menu == 1)
    {
      menu = 2;
      thick = opt;
    }
    if(menu == 2)
    {
      if(opt == 1)
      {
        menu = 3;
        heaters();
        vacuum();
      }
      if(opt == 2)
      {
        menu = 0;
      }
    }
    if(menu == 3)
    {
      if(opt == 1)
      {
        menu = 3;
        heaters();
        vacuum();
      }
      if(opt == 2)
      {
        menu = 0;
      }
    }
  }
  
}

void heaters() {
  //Jahein part
  lcd.clear();
  lcd.print("Put the sheet");
do {
  for(int i=0;i<4;i++){
  irRead[i] = digitalRead(irPin[i]);
}
  
}while(irRead[0]==0 || irRead[1]==0 || irRead[2]==0 || irRead[3]==0);

int limitStart = digitalRead(limitSwitch[0]);
  lcd.clear();
  lcd.print("Pull the Heater");
do{
  int limitStart = digitalRead(limitSwitch[0]);
}while(limitStart == 0);
  
  digitalWrite(heatRelay , HIGH);
  
for(heatTimeCount=0; heatTimecount <= heatTime * 4; heatTimeCount++){
  temp = thermocouple.readCelsius();
  if(temp>tempMax){
    digitalWrite(heatRelay , LOW);
  }
  else if(temp<tempMin){
    digitalWrite(heatRelay , HIGH);
  }
  delay(250);
}

    digitalWrite(heatRelay , LOW);
    lcd.clear();
    lcd.print("Push heaters");
    digitalWrite(buzzer,HIGH);
  }
  
  int limitEnd = digitalRead(limitSwitch[1]);
  lcd.clear();
  lcd.print("Push the Heater");
  do{
  int limitEnd = digitalRead(limitSwitch[1]);
  digitalWrite(buzzer , HIGH);
}while(limitEnd == 0);

digitalWrite(buzzer , LOW);
}







void vacuum() {
  //Osman part
  lcd.clear();
  lcd.setCursor(1,1);  
  lcd.print ("Pull up the Platen");
  lcd.setCursor(6,2);  
  lcd.print ("Please..");
   
  do {
    int limitSwitchState = digitalRead(limitSwitch[2]);
    lamp[2].blink(500, 500);
    }
while (limitSwitchState == 0);  
    digitalWrite(vacuum,LOW);
    digitalWrite(lamp[2], HIGH); 
 for(int i=20 ; i>0 ; i--){
  
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Vacuum Process in progress");
  lcd.setCursor(4,2);
  lcd.print("Countdown ");
  lcd.print(i); 
  if(i==8){
  lcd.print("Countdown: ");
  lcd.print(i);
  if(last_counter==8){
    digitalWrite(fan,HIGH);
    digitalWrite(lamp[3],LOW);
    }
 if (i==5){
  digitalWrite(vacuum,HIGH);
  }
 if(i==0){
  digitalWrite(fan,LOW);
  digitalWrite(lamp[3],HIGH);
 }
 }
    lcd.clear();
    lcd.setCursor(1,1);  
    lcd.print ("Sheet is ready..");
    sheetCount=sheetCount++;
    lcd.setCursor(3,2);
    lcd.print("Take it now..");
    
  do{ 
      irRead[i] = digitalRead(irPin[i]);
  }
  while(irRead[0]==1 && irRead[1]==1 && irRead[2]==1 && irRead[3]==1);
}
