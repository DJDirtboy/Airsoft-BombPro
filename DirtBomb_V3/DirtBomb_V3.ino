#include <Wire.h> 
#include <Keypad.h>
//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
/*
 Arduino Bomb Pro
 
 The circuit:
 * More info at : http://yin.mainstreamds.com/
 If you need some help mail me to yinbot@gmail.com
 
 created 4,Sep, 2010
 Modified 24 May 2014
 by Ignacio Lillo
 modified 16 May 2020 by DJDirtboy
 
 */

//LiquidCrystal_I2C lcd(0x38,16,2);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
const int LCD_NB_ROWS = 2;
const int LCD_NB_COLUMNS = 16;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','a' }
  ,
  {'4','5','6','b' }
  ,
  {'7','8','9','c' }
  ,
  {'*','0','#','d' }
};

byte rowPins[ROWS] = {A0, A1, A2, A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A4, A5, 12, 13}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


char enteredText[8];
byte time[4];
byte refresh=0;//1 if is refreshed once time...
char password[8];
int key=-1;
char lastKey;
char var;
boolean passwordEnable=false;

//Buttons for lcd shield
char BT_RIGHT = '4';
char BT_UP = 'a';
char BT_DOWN = 'b';
char BT_LEFT = '6';
char BT_SEL = 'd';   // Ok key  
char BT_CANCEL = 'c';
char BT_DEFUSER = 'x';   // not implemented

//leds
const int REDLED = 11;
const int GREENLED = 10;
//const int BLUELED = 12;
//mosfet
boolean mosfetEnable = false;
const int mosfet = 9;
//IS VERY IMPORTANT THAT YOU TEST THIS TIME. BY DEFAULT IS IN 1 SEC. THAT IS NOT TOO MUCH. SO TEST IT!
const int MOSFET_TIME = 5000;


//TIME INTS
int GAMEHOURS = 0;
int GAMEMINUTES = 45;
int BOMBMINUTES = 4;
int ACTIVATESECONDS = 5;

boolean endGame = false;

boolean sdStatus = false; //search and destroy game enable used in config
boolean saStatus = false; //same but SAbotage
boolean doStatus = false; //for DEmolition
boolean start = true;
boolean defuseando;
boolean cancelando;
// SOUND TONES
boolean soundEnable = true;
int tonepin = 8; // Pin for the buzzer
int tonoPitido = 3000;
int tonoAlarma1 = 700;
int tonoAlarma2 = 2600;
int tonoActivada = 1330;
int errorTone = 100;

unsigned long iTime;
unsigned long timeCalcVar;
unsigned long redTime;
unsigned long greenTime;
unsigned long iZoneTime;//initial time for zone
byte team=0; // 0 = neutral, 1 = green team, 2 = red team

void setup(){
  lcd.begin(16, 2);
  Serial.begin(9600);
  //  lcd.init();                      // initialize the lcd 
  //  lcd.backlight();
  lcd.setCursor(3,0);
  tone(tonepin,2400,30);
  lcd.print(" DJDIRTBOY");// you can add your team name or someting cool
  lcd.setCursor(0,1);
  lcd.print("     AIRSOFT");// you can add your team name or someting cool
  keypad.setHoldTime(50);
  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent);
  delay(3000);
  pinMode(GREENLED, OUTPUT);     
//  pinMode(8, OUTPUT);  
//  digitalWrite(8,HIGH);
  pinMode(REDLED, OUTPUT); 
  pinMode(mosfet, OUTPUT);  

/* Caractères personnalisés pour la barre de progression et les curseurs  */
byte START_DIV_0_OF_1[8] = {
  B01111, 
  B11000,
  B10000,
  B10000,
  B10000,
  B10000,
  B11000,
  B01111
}; // Char début 0 / 1

byte START_DIV_1_OF_1[8] = {
  B01111, 
  B11000,
  B10011,
  B10111,
  B10111,
  B10011,
  B11000,
  B01111
}; // Char début 1 / 1

byte DIV_0_OF_2[8] = {
  B11111, 
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
}; // Char milieu 0 / 2

byte DIV_1_OF_2[8] = {
  B11111, 
  B00000,
  B11000,
  B11000,
  B11000,
  B11000,
  B00000,
  B11111
}; // Char milieu 1 / 2

byte DIV_2_OF_2[8] = {
  B11111, 
  B00000,
  B11011,
  B11011,
  B11011,
  B11011,
  B00000,
  B11111
}; // Char milieu 2 / 2

byte END_DIV_0_OF_1[8] = {
  B11110, 
  B00011,
  B00001,
  B00001,
  B00001,
  B00001,
  B00011,
  B11110
}; // Char fin 0 / 1

byte END_DIV_1_OF_1[8] = {
  B11110, 
  B00011,
  B11001,
  B11101,
  B11101,
  B11001,
  B00011,
  B11110
}; // Char fin 1 / 1

  byte up[8] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B00000,
    B00000,
  };

  byte down[8] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
  };


  /* Enregistre les caractères personnalisés dans la mémoire de l'écran LCD DJDirtboy*/
  lcd.createChar(9, START_DIV_0_OF_1);
  lcd.createChar(1, START_DIV_1_OF_1);
  lcd.createChar(2, DIV_0_OF_2);
  lcd.createChar(3, DIV_1_OF_2);
  lcd.createChar(4, DIV_2_OF_2);
  lcd.createChar(5, END_DIV_0_OF_1);
  lcd.createChar(6, END_DIV_1_OF_1);
  lcd.createChar(7,up);
  lcd.createChar(8,down);
}



void loop(){
  menuPrincipal();
}
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case PRESSED:
      switch (key){

      }
    break;
    case RELEASED:
      switch (key){
         case 'd': defuseando= false;
         //Serial.println("d Releases");
         break;
         case 'c': cancelando=false;
         //Serial.println("c Releases");
         break;
      }
    break;
    case HOLD:
      switch (key){
        case 'd': defuseando= true;
        //Serial.println("d hold");
        break;
        case 'c': cancelando=true;
        //Serial.println("c hold");
        break;
      }
    break;
  }
}

void disarmedSplash(){
  endGame = false;
  digitalWrite(REDLED, LOW); 
  digitalWrite(GREENLED, LOW);
  if(sdStatus || saStatus){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("BOMB DISARMED");
    lcd.setCursor(3,1);
    lcd.print("GOODS WIN");
    digitalWrite(REDLED, HIGH); //DJDirtboy LED Test
    digitalWrite(GREENLED, LOW);  
    delay(5000);
    digitalWrite(GREENLED, HIGH); 
  }
  //end code
  lcd.clear();
  lcd.print("Play Again?");
  lcd.setCursor(0,1);
  lcd.print("A : Yes B : No");
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      //We have two options, search & destroy and sabotaje play again options so!
      if(sdStatus){
        startGameCount();
        search();
      }
      if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; //to set iTime to actual millis() :D
        sabotage();
      }
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();
      break;
    }  
  } 
}

void explodeSplash(){
  digitalWrite(REDLED, HIGH);  
  digitalWrite(GREENLED, HIGH); 
  cls();
  delay(100);
  endGame = false;
  lcd.setCursor(1,0);
  lcd.print("TERRORISTS WIN");
  lcd.setCursor(4,1);
  lcd.print("GAME OVER");
  digitalWrite(REDLED, LOW); //DJDirtboy redled test
  for(int i = 200; i>0; i--)// this is the ultra hi definition explosion sound xD
  {
    tone(tonepin,i);
    delay(20);
  }
  noTone(tonepin);
  if(mosfetEnable){
    activateMosfet(); 
  }
  delay(5000);
  cls();

  //end code
  lcd.print("Play Again?");
  lcd.setCursor(0,1);
  lcd.print("A : Yes B : No");
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      //We have two options, search & destroy and sabotaje play again options so!
      if(sdStatus){
        startGameCount();
        search();
      }
      if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; //to set iTime to actual millis() :D
        sabotage();
      }
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();

      break;
    }  
  } 
}
