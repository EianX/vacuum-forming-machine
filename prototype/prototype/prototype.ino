/*
 *- Project neame: Vacuum forming machine prototype.
 *- Programmers: Abdelrahman Nasr Maaroof, Mahmoud Ali Abdalmonem, Mahmoud Ibrahim Osman.
 *- Revision history: V 
 *- Description: Manual vacuum forming machine used to shape plastic materials. During the vacuum forming process, a sheet of plastic is heated and then pulled around a single mould using suction.
 *- Development details:
      MCU: Arduino mega, SW: Arduino IDE.
 *- Notes: A program to take the user input fot the sheet material type and its thickness then start the vacuum forming process with heating the sheet first then vacuum air to shape it after the placed Mould
           with all the information and instructions displayed on LCD and 4 lamps for indications.
 */

//Including required libraries:
#include <LiquidCrystal.h> //LCD library included with arduine IDE
#include <max6675.h> //Thermocouple Temperature Sensor K Type with the Maxim MAX6675 K-Thermocouple to digital converter IC library by Adafruit
#include <Encoder.h> //Encoder library by Paul Stoffregen
//Pins variables:
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //LCD pins (rs, en, d4, d5, d6, d7)
Encoder myEnc(, ); //Encoder signal pins (interrupt pins for best performance)
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO); //Thermocouple Temperature Sensor K Type signal pins (thermoCLK, thermoCS, thermoDO)
const int encButton = ; //Encoder push button
const int lamp[2] = { , }; //Index(0): for vacuum lamp, Index(1): for fans lamp
const int heatRelay = ; //For heaters and its lamp
const int irPin[4] = {, , , }; //4 IR sensors
const int buzzer = ;
const int limitSwitch[3]={, , }; //Index(0,1): start and end for heaters, Index(2): end for platen
const int fan = ;
const int vacuum = ; //The vacuum source relay
//Data variables:
int material = 0; //Acrylic-PVC:0, PP:1
String materialName = "";
int thick = 0;
float temp = 0.0;
float tempMax = 0.0, tempMin = 0.0; //The temperature range for forming the sheet
int heatTime = 0; //The time needed for the sheet to be formed
int heatTimeCount = 0; 
int sheetCount = 0;
int vacuumTime = 20; //The vacuum time
int irRead[4]={, , , }; //The 4 IR sensors reads
//LCD Menues variables:
long counter = 0; //Encoder position
int menu = 0;
int opt = 0;
long lastCounter = -999;
bool pushed = true;
//Material and thickness heat forming data:
const float tempRange[2][2] = {{140.0, 190.0}, {150.0, 180.0}}; //tempMin = tempRange[material][0], tempMax = tempRange[material][1]
const int thickTime[2][4]={{0, 30, 60, 90}, {0, 50, 100, 150}}; //heatTime = thickTime[material][thick]
//LCD special characters:
uint8_t arrow[8] = {0x0, 0x04, 0x06, 0x1f, 0x06, 0x04, 0x00, 0x00}; //Arrow char
uint8_t checked[8] = {0x1F, 0x13, 0x13, 0x13, 0x1B, 0x1B, 0x15, 0x1F}; //Checked char


void setup() {
  // put your setup code here, to run once:
  //The pins mode (Input or Output)
  pinMode(encButton, INPUT);
  pinMode(heatRelay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(vacuum, OUTPUT);
  for(int i = 0; i < 4; i++){
    if(i < 2)
    {
      pinMode(lamp[i], OUTPUT);
    }
    if(i < 3)
    {
      pinMode(limitSwitch[i], INPUT);
    }
    
    pinMode(irPin[i], INPUT);
  }
  
  //Initialize the LCD
  lcd.begin(20, 4);
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight
  lcd.createChar(0, arrow);   //Create the arrow symbol
  lcd.createChar(1, checked); //Create the checked symbol
  lcd.home();                 //Home the LCD

  //Display the welcome message
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
    //2nd menu (choose thickness: 1mm, 2mm or 3mm):
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
    //3rd menu (comformition step)
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
    //4th menu (repeat or return to options)
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
  //The menus different options actions
  if(digitalRead(encButton)){
    pushed = true;
    myEnc.write(0);
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
    if(menu == 1)
    {
      menu = 2;
      thick = opt;
    }
    //3rd and 4th menus preform the vacuum forming process 
    if(menu == 2 || menu == 3)
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
  //The heaters section:
  //Display place the sheet instruction
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Push back the Heater");
  lcd.setCursor(0,1);
  lcd.print("Then place the Sheet");
  lcd.setCursor(6,3);
  lcd.print("Please..");
  //Read the 4 IR sensors till the sheet is placed right
  do {
    for(int i = 0; i < 4; i++){
      irRead[i] = digitalRead(irPin[i]);
    }
  }while(irRead[0] == 0 || irRead[1] == 0 || irRead[2] == 0 || irRead[3] == 0);
  //Display pull the heaters instruction
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Pull the Heaters");
  lcd.setCursor(2,2);
  lcd.print("forward Please..");
  //Read the start limit switch for the heaters till it reaches
  do{
    int limitStart = digitalRead(limitSwitch[0]);
  }while(limitStart == 0);
  //Wait till the heaters warm up  
  digitalWrite(heatRelay , HIGH);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Heater is warming up");
  lcd.setCursor(3,2);
  lcd.print("Please Wait...");

  tempMin = tempRange[material][0];
  tempMax = tempRange[material][1];
  heatTime = thickTime[material][thick];
  
  do{
    temp = thermocouple.readCelsius();
  }while(temp < tempMin);
  //The forming time countdown
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Forming  Temperature");
  lcd.setCursor(2,1);
  lcd.print("has been reached");
  lcd.setCursor(3,3);
  lcd.print("Countdown : ");
  lcd.print(heatTime)
  
  for(heatTimeCount = 0; heatTimeCount <= heatTime * 4; heatTimeCount++){
    temp = thermocouple.readCelsius();
    if(heatTimeCount % 4 == 0)
    {
      lcd.setCursor(12,3);
      lcd.print("     ");
      lcd.setCursor(12,3);
      lcd.print(heatTime - (heatTimeCount / 4));
    }
    if(temp > tempMax)
    {
      digitalWrite(heatRelay , LOW);
    }
    else if(temp < tempMin)
    {
      digitalWrite(heatRelay , HIGH);
    }
    delay(250);
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
  //Read the end limit switch for the heaters till it reaches
  do{ 
    int limitEnd = digitalRead(limitSwitch[1]);
    for(int i = 50; i < 256; i++){
      analogWrite(buzzer, i);
      delay(10);
    }
  }while(limitEnd == 0);
  //Turn off buzzer after completing
  analogWrite(buzzer, 0);
}



  
void vacuum() {
  //The vacuum section:
  //Display pull up the platen instruction
  lcd.clear();
  lcd.setCursor(1,1);  
  lcd.print ("Pull up the Platen");
  lcd.setCursor(6,2);  
  lcd.print ("Please..");
  //Read the end limit switch for the platen till it reaches       
  do {
    int limitSwitchState = digitalRead(limitSwitch[2]);
    digitalWrite(lamp[0], (millis() / 1000) % 2);
  }while(limitSwitchState == 0);  
  //Trun on the vacuum and its lamp after completing
  digitalWrite(vacuum, LOW);
  digitalWrite(lamp[0], HIGH); 
  //Vacuum time countdown   
  for(int i = vacuumTime; i > 0; i--){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Vacuum Process");
    lcd.setCursor(3,1);
    lcd.print("in progress ..");
    lcd.setCursor(3,2);
    lcd.print("Countdown : ");
    lcd.print(i); 
    //Turn on fans and its lamp
    if(i == 8)
    {
      digitalWrite(fan, HIGH);
      digitalWrite(lamp[1], LOW);
    }
    //Turn off vacuum source and its lamp
    if(i == 5)
    {
      digitalWrite(vacuum, HIGH);
      digitalWrite(lamp[0], LOW); 
    }
    //Turn off fans and its lamp
    if(i == 0)
    {
      digitalWrite(fan, LOW);
      digitalWrite(lamp[1], HIGH);
    }
   delay(1000);
 }
  //Display take out the sheet instruction
  lcd.clear();
  lcd.setCursor(3,1);  
  lcd.print ("Sheet is Ready");
  lcd.setCursor(1,2);
  lcd.print("You can take it ..");
  sheetCount++;
  //Wait till the sheet is taken out  
  do{
    for(int i = 0; i < 4; i++){
      irRead[i] = digitalRead(irPin[i]);
    }
  }while(irRead[0] == 1 && irRead[1] == 1 && irRead[2] == 1 && irRead[3] == 1);
}
