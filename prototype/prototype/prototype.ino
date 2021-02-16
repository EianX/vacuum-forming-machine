/*
 *- Project neame: Vacuum forming machine prototype.
 *- Programmers: Abdelrahman Nasr Maaroof, Mahmoud Ali Abdalmonem, Mahmoud Ibrahim Osman.
 *- Revision history: V4.6 
 *- Description: Manual vacuum forming machine used to shape plastic materials. During the vacuum forming process, a sheet of plastic is heated and then pulled around a single mould using suction.
 *- Development details:
      MCU: Arduino Mega, SW: Arduino IDE.
 *- Notes: A program to take the user input fot the sheet material type and its thickness then start the vacuum forming process with heating the sheet first then vacuum air to shape it after the 
           placed Mould with all the information and instructions displayed on LCD and 4 lamps for indications.
 */

//Including required libraries:
#include <LiquidCrystal.h> //LCD library included with arduine IDE
#include <max6675.h> //Thermocouple Temperature Sensor K Type with the Maxim MAX6675 K-Thermocouple to digital converter IC library by Adafruit
#include <Encoder.h> //Encoder library by Paul Stoffregen
//Pins variables:
LiquidCrystal lcd(29, 31, 37, 39, 41, 43); //LCD pins (rs, en, d4, d5, d6, d7)
Encoder myEnc(18, 19); //Encoder signal pins (interrupt pins for best performance)
MAX6675 thermocouple(4, 3, 2); //Thermocouple Temperature Sensor K Type signal pins (thermoCLK, thermoCS, thermoDO)
const int encButton = 17; //Encoder push button
const int lamp[2] = {45, 35}; //Index(0): for vacuum lamp, Index(1): for fans lamp
const int heatRelay = 21; //For heaters and its lamp
const int buzzer = 11;
const int limitSwitch[3]={27, 23, 25}; //Index(0,1): start and end for heaters, Index(2): end for platen
const int fan = 10;
const int vacuum = 20; //The vacuum source relay
//Data variables:
int material = 0; //Acrylic-PVC:0, PP:1
String materialName = "";
int thick = 0;
String thickName = "";
float temp = 0.0;
float tempMax = 0.0, tempMin = 0.0; //The temperature range for forming the sheet
int heatTime = 0; //The time needed for the sheet to be formed
int heatTimeCount = 0; 
int sheetCount = 0;
int vacuumTime = 25; //The vacuum time
//LCD Menues variables:
long counter = 0; //Encoder position
int menu = 0;
int opt = 0;
long lastCounter = -999;
bool pushed = true;
//Material and thickness heat forming data:
const float tempRange[2][2] = {{150.0, 190.0}, {150.0, 180.0}}; //tempMin = tempRange[material][0], tempMax = tempRange[material][1]
const int thickTime[2][4]={{0, 19, 29, 39}, {0, 20, 30, 40}}; //heatTime = thickTime[material][thick]
//LCD special characters:
uint8_t arrow[8] = {0x00, 0x04, 0x06, 0x1f, 0x06, 0x04, 0x00, 0x00}; //Arrow char
uint8_t checked[8] = {0x00, 0x10, 0x10, 0x14, 0x12, 0x1F, 0x02, 0x04}; //Checked char
uint8_t slash[8] = {0x00, 0x10, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00}; //Backslash char


void setup() {
  // put your setup code here, to run once:
  //The pins mode (Input or Output)
  pinMode(encButton, INPUT);
  pinMode(heatRelay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(vacuum, OUTPUT);
  for(int i = 0; i < 3; i++){
    if(i < 2)
    {
      pinMode(lamp[i], OUTPUT);
      digitalWrite(lamp[i], HIGH);
    }
    
    pinMode(limitSwitch[i], INPUT);
  }
  //Initialize the outputs
  digitalWrite(heatRelay, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(fan, LOW);
  digitalWrite(vacuum, HIGH);
  //Initialize the LCD
  lcd.begin(20, 4);
  lcd.createChar(1, checked); //Create the checked symbol
  lcd.createChar(2, arrow);   //Create the arrow symbol
  lcd.createChar(3, slash); //Create the checked symbol
  lcd.home();                 //Home the LCD

  //Display the welcome message
  lcd.clear();
  lcd.setCursor(3, 0);  
  lcd.print("Vacuum Forming");
  lcd.setCursor(2, 1);  
  lcd.print("Machine Prototype");
  lcd.setCursor(5, 3);  
  lcd.print("WELCOME :D");
  delay(3000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  counter = myEnc.read(); //Read the encoder position

  //Limit the position range
  if(counter > 17){
    counter = 17;
  }
  if(counter < 0){
    counter = 0;
  }
  //Display menus when encoder rotate or is pushed:
  if(counter != lastCounter || pushed){
    pushed = false;
    //1st menu (choose material: PVC, Acrylic or PP):
    if(menu == 0)
    {  
      if(0 <= counter && counter < 6)
      {
        lcd.setCursor(0,0);
        lcd.print("Select material:");
        lcd.setCursor(0,1);
        lcd.write(2);  
        lcd.print("PVC");
        lcd.setCursor(0,2);  
        lcd.print(" Acrylic");
        lcd.setCursor(0,3);  
        lcd.print(" PP");
        opt = 1;
      }
      if(6 <= counter && counter < 12)
      {
        lcd.setCursor(0,0);
        lcd.print("Select material:");
        lcd.setCursor(0,1);  
        lcd.print(" PVC");
        lcd.setCursor(0,2); 
        lcd.write(2); 
        lcd.print("Acrylic");
        lcd.setCursor(0,3);  
        lcd.print(" PP");
        opt = 2;
      }
      if(12 <= counter && counter < 18)
      {
        lcd.setCursor(0,0);
        lcd.print("Select material:");
        lcd.setCursor(0,1);  
        lcd.print(" PVC");
        lcd.setCursor(0,2);  
        lcd.print(" Acrylic");
        lcd.setCursor(0,3); 
        lcd.write(2); 
        lcd.print("PP");
        opt = 3;
      }
    }
    //2nd menu (choose thickness: 1mm, 2mm or 3mm):
    if(menu == 1)
    {  
      if(0 <= counter && counter < 6)
      {
        lcd.setCursor(0,0);
        lcd.print("Select thickness:");
        lcd.setCursor(0,1);
        lcd.write(2);  
        lcd.print("300 um");
        lcd.setCursor(0,2);  
        lcd.print(" 650 um");
        lcd.setCursor(0,3);  
        lcd.print(" 1 mm");
        opt = 1;
      }
      if(6 <= counter && counter < 12)
      {
        lcd.setCursor(0,0);
        lcd.print("Select thickness:");
        lcd.setCursor(0,1);  
        lcd.print(" 300 um");
        lcd.setCursor(0,2); 
        lcd.write(2); 
        lcd.print("650 um");
        lcd.setCursor(0,3);  
        lcd.print(" 1 mm");
        opt = 2;
      }
      if(12 <= counter && counter < 18)
      {
        lcd.setCursor(0,0);
        lcd.print("Select thickness:");
        lcd.setCursor(0,1);  
        lcd.print(" 300 um");
        lcd.setCursor(0,2);  
        lcd.print(" 650 um");
        lcd.setCursor(0,3); 
        lcd.write(2); 
        lcd.print("1 mm");
        opt = 3;
      }
    }
    //3rd menu (comformition step)
    if(menu == 2)
    {  
      if(0 <= counter && counter < 9)
      {
        lcd.setCursor(0,0);
        lcd.write(1);
        lcd.print("Material: ");
        lcd.print(materialName);
        lcd.setCursor(0,1);
        lcd.write(1);  
        lcd.print("Thickness: ");
        lcd.print(thickName);
        lcd.setCursor(0,2); 
        lcd.write(2); 
        lcd.print("Continue");
        lcd.setCursor(0,3);  
        lcd.print(" Return");
        opt = 1;
      }
      if(9 <= counter && counter < 18)
      {
        lcd.setCursor(0,0);
        lcd.write(1);
        lcd.print("Material: ");
        lcd.print(materialName);
        lcd.setCursor(0,1);
        lcd.write(1);  
        lcd.print("Thickness: ");
        lcd.print(thickName);
        lcd.setCursor(0,2);  
        lcd.print(" Continue");
        lcd.setCursor(0,3);
        lcd.write(2);  
        lcd.print("Return");
        opt = 2;
      }
    }
    //4th menu (repeat or return to options)
    if(menu == 3)
    {  
      if(0 <= counter && counter < 9)
      {
        lcd.setCursor(0,0);
        lcd.print("Total sheet count:");
        lcd.setCursor(0,1);
        lcd.write(1);  
        lcd.print("  ");
        lcd.print(sheetCount);
        lcd.print("  sheets      ");
        lcd.setCursor(0,2);
        lcd.write(2);  
        lcd.print("Make another sheet");
        lcd.setCursor(0,3);  
        lcd.print(" Return to options");
        opt = 1;
      }
      if(9 <= counter && counter < 18)
      {
        lcd.setCursor(0,0);
        lcd.print("Total sheet count:");
        lcd.setCursor(0,1);
        lcd.write(1);  
        lcd.print("  ");
        lcd.print(sheetCount);
        lcd.print("  sheets      ");
        lcd.setCursor(0,2);  
        lcd.print(" Make another sheet");
        lcd.setCursor(0,3); 
        lcd.write(2); 
        lcd.print("Return to options");
        opt = 2;
      }
    }
  }

  lastCounter = counter;
  //The menus different options actions
  if(!digitalRead(encButton)){
    delay(500);
    pushed = true;
    myEnc.write(0);
    lcd.clear();
    //1st menu save the material number and name
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
    //2nd menu save the thickness
    else if(menu == 1)
    {
      menu = 2;
      thick = opt;
      if(opt == 1)
      {
        thickName = "300 um";
      }
      if(opt == 2)
      {
        thickName = "650 um";
      }
      if(opt == 3)
      {
        thickName = "1 mm";
      }
    }
    //3rd and 4th menus preform the vacuum forming process 
    else if(menu == 2 || menu == 3)
    {
      if(opt == 1)
      {
        menu = 3;
        heaters();
        Vacuum();
      }
      if(opt == 2)
      {
        menu = 0;
      }
    }
  }
  
}

void heaters() {
  //The heaters section:
  tempMin = tempRange[material][0];
  tempMax = tempRange[material][1];
  heatTime = thickTime[material][thick];
  //Display place the sheet instruction
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Push back the Heater");
  lcd.setCursor(0,1);
  lcd.print("Then place the Sheet");
  lcd.setCursor(6,2);
  lcd.print("Please..");
  lcd.setCursor(0,3);
  lcd.write(2);
  lcd.print("Continue");
  //Read the limit switch till the heaters are pushed
  while(digitalRead(encButton) || !digitalRead(limitSwitch[1]));
  //Wait till the heaters warm up  
  digitalWrite(heatRelay , HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Heater is warming up");
  lcd.setCursor(3,1);
  lcd.print("Please Wait...");
  lcd.setCursor(0,3);
  lcd.print("Temp: ");
  lcd.print(tempMin);
  lcd.write(3);
  do{
    temp = thermocouple.readCelsius();
    lcd.setCursor(13,3);
    lcd.print(temp);
    delay(500);
    lcd.print(" ");
  }while(temp < tempMin);
  //Display pull the heaters instruction
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Pull the Heaters");
  lcd.setCursor(2,2);
  lcd.print("forward Please..");
  //Turn on the buzzer and read the start limit switch for the heaters till it reaches
  do{ 
    digitalWrite(buzzer, HIGH);
    delay(350);
    digitalWrite(buzzer, LOW);
    delay(350);
  }while(!digitalRead(limitSwitch[0]));
  //Turn off buzzer after completing
  digitalWrite(buzzer, 0);
  //The forming time countdown
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Forming  Temperature");
  lcd.setCursor(2,1);
  lcd.print("has been reached");
  lcd.setCursor(3,3);
  lcd.print("Countdown : ");
  lcd.print(heatTime);
  
  for(heatTimeCount = 0; heatTimeCount <= heatTime * 2; heatTimeCount++){
    temp = thermocouple.readCelsius();
    delay(500);
    if(heatTimeCount % 2 == 0)
    {
      lcd.setCursor(15,3);
      lcd.print(heatTime - (heatTimeCount / 2));
      lcd.print("  ");
    }
    if(temp > tempMax)
    {
      digitalWrite(heatRelay , LOW);
    }
    else if(temp < tempMin)
    {
      digitalWrite(heatRelay , HIGH);
    }
  }
  //Turn off heaters after completing
  digitalWrite(heatRelay , LOW);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Heating  process");
  lcd.setCursor(1,1);
  lcd.print("has been completed");
  lcd.setCursor(0,2);
  lcd.print("Push back the Heater");
  lcd.setCursor(3,3);
  lcd.print("again Please..");
  //Turn on the buzzer and read the end limit switch for the heaters till it reaches
  do{ 
    digitalWrite(buzzer, HIGH);
    delay(350);
    digitalWrite(buzzer, LOW);
    delay(350);
  }while(!digitalRead(limitSwitch[1]));
  //Turn off buzzer after completing
  digitalWrite(buzzer, 0);
}



  
void Vacuum() {
  //The vacuum section:
  //Display pull up the platen instruction
  lcd.clear();
  lcd.setCursor(1,1);  
  lcd.print("Pull up the Platen");
  lcd.setCursor(6,2);  
  lcd.print("Please..");
  //Read the end limit switch for the platen till it reaches       
  while(!digitalRead(limitSwitch[2]));  
  //Trun on the vacuum and its lamp after completing
  digitalWrite(vacuum, LOW);
  digitalWrite(lamp[0], LOW);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Vacuum Process");
  lcd.setCursor(3,1);
  lcd.print("in progress ..");
  lcd.setCursor(3,2);
  lcd.print("Countdown : ");
  lcd.begin(20, 4);
  //Vacuum time countdown   
  for(int i = vacuumTime; i >= 0; i--){
    //Turn on fans and its lamp
    if(i == 10)
    {
      digitalWrite(fan, HIGH);
      digitalWrite(lamp[1], LOW);
      lcd.begin(20, 4);
    }
    //Turn off vacuum source and its lamp
    if(i == 5)
    {
      digitalWrite(vacuum, HIGH);
      digitalWrite(lamp[0], HIGH);
      lcd.begin(20, 4); 
    }
    lcd.begin(20, 4);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("   Vacuum Process   ");
    lcd.setCursor(0,1);
    lcd.print("   in progress ..   ");
    lcd.setCursor(0,2);
    lcd.print("   Countdown :      "); 
    lcd.setCursor(15,2);
    lcd.print(i);
    lcd.setCursor(0,3);
    lcd.print("                    "); 
    delay(1000);
  }
  delay(500);
  //Display take out the sheet instruction
  lcd.begin(20, 4);
  lcd.createChar(1, checked); 
  lcd.createChar(2, arrow);   
  lcd.createChar(3, slash); 
  lcd.clear();
  lcd.setCursor(3,1);  
  lcd.print ("Sheet is Ready");
  lcd.setCursor(1,2);
  lcd.print("You can take it ..");
  lcd.setCursor(0,3);
  lcd.write(2);
  lcd.print("Continue");
  sheetCount++;
  //Wait till the sheet is taken out  
  while(digitalRead(encButton));
  //Turn off fans and its lamp
  digitalWrite(fan, LOW);
  digitalWrite(lamp[1], HIGH);
  lcd.begin(20, 4);
  lcd.createChar(1, checked); 
  lcd.createChar(2, arrow);   
  lcd.createChar(3, slash); 
  delay(500);
}
