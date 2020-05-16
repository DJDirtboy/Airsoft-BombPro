

/*
 * Fonction dessinant la barre de progression.
 */
void drawPorcent(byte percent) {
 
  /* Affiche la nouvelle valeur sous forme numérique sur la première ligne */
  //lcd.setCursor(0, 0);
  //lcd.print(percent);
  //lcd.print(F(" %  ")); 
  // N.B. Les deux espaces en fin de ligne permettent d'effacer les chiffres du pourcentage
  // précédent quand on passe d'une valeur à deux ou trois chiffres à une valeur à deux ou un chiffres.
 
  /* Déplace le curseur sur la seconde ligne */
  lcd.setCursor(0, 1);
 
  /* Map la plage (0 ~ 100) vers la plage (0 ~ LCD_NB_COLUMNS * 2 - 2) */
  byte nb_columns = map(percent, 0, 100, 0, LCD_NB_COLUMNS * 2 - 2);
  // Chaque caractère affiche 2 barres verticales, mais le premier et dernier caractère n'en affiche qu'une.

  /* Dessine chaque caractère de la ligne */
  for (byte i = 0; i < LCD_NB_COLUMNS; ++i) {

    if (i == 0) { // Premiére case

      /* Affiche le char de début en fonction du nombre de colonnes */
      if (nb_columns > 0) {
        lcd.write(1); // Char début 1 / 1
        nb_columns -= 1;

      } else {
        lcd.write((byte) 9); // Char début 0 / 1
      }

    } else if (i == LCD_NB_COLUMNS -1) { // Derniére case

      /* Affiche le char de fin en fonction du nombre de colonnes */
      if (nb_columns > 0) {
        lcd.write(6); // Char fin 1 / 1

      } else {
        lcd.write(5); // Char fin 0 / 1
      }

    } else { // Autres cases

      /* Affiche le char adéquat en fonction du nombre de colonnes */
      if (nb_columns >= 2) {
        lcd.write(4); // Char div 2 / 2
        nb_columns -= 2;

      } else if (nb_columns == 1) {
        lcd.write(3); // Char div 1 / 2
        nb_columns -= 1;

      } else {
        lcd.write(2); // Char div 0 / 2
      }
    }
  }
}
void cls(){
  lcd.clear();
  lcd.setCursor(0,0);
}

void printTime(unsigned long minutos, unsigned long aTiempo){

  timeCalcVar=(minutos-aTiempo/60000);
  //Hours
  
  if(timeCalcVar/60<1 && refresh==0){
      lcd.clear();
      refresh=1;
      delay(100);
      lcd.setCursor(3,1);
  }
   
  if(timeCalcVar/60>=1){
    
    if(timeCalcVar/60<10)
  {
    
    lcd.setCursor(2,1);
    lcd.print("0");
    lcd.print(timeCalcVar/60);
  }
  else
  {
    lcd.print(timeCalcVar/60);
  }
  
  lcd.print(":");
  
  }
  //minutes
  if((timeCalcVar%60)<10)
  {
    lcd.print("0");
    lcd.print(timeCalcVar%60);
  }
  else
  {
    lcd.print(timeCalcVar%60);
  }
  lcd.print(":");
  //seconds
  timeCalcVar=(aTiempo/1000);
  if((59-(timeCalcVar%60))<10)
  {
    lcd.print("0");
    lcd.print(59-(timeCalcVar%60));
  }
  else
  {
    lcd.print(59-(timeCalcVar%60));
  }
  lcd.print(":");
  //this not mach with real time, is just a effect, it says 999 because millis%1000 sometimes give 0 LOL
  lcd.print(999-(millis()%1000));
}

void printTimeDom(unsigned long aTiempo, boolean showMillis){
  //minutes
  if((aTiempo/60000)<10)
  {
    lcd.print("0");
    lcd.print(aTiempo/60000);
  }
  else
  {
    lcd.print(aTiempo/60000);
  }
  lcd.print(":");
  //seconds
  if(((aTiempo/1000)%60)<10)
  {
    lcd.print("0");
    lcd.print((aTiempo/1000)%60);
  }
  else
  {
    lcd.print((aTiempo/1000)%60);
  }
  if(showMillis){
    lcd.print(":");
    //this not mach with real time, is just a effect, it says 999 because millis%1000 sometimes give 0 LOL
      lcd.print(999-millis()%1000);

  }
}


void startGameCount(){
  cls();
  lcd.setCursor(1,0);
  lcd.print("Ready to Begin");
  lcd.setCursor(0,1);
  lcd.print("Push ANY Button");
  keypad.waitForKey();//if you press a button game start

  cls();
  lcd.setCursor(1,0);
  lcd.print("Starting Game");
  for(int i = 5; i > 0 ; i--){ // START COUNT GAME INIT
    lcd.setCursor(5,1);
    tone(tonepin,2000,100);
    lcd.print("IN ");
    lcd.print(i);
    delay(1000);
  }
  cls();
}

void checkArrows(byte i,byte maxx ){

  if(i==0){
    lcd.setCursor(15,1);
    lcd.write(8); 
  }
  if(i==maxx){  
    lcd.setCursor(15,0);
    lcd.write(7);
  }
  if(i>0 && i<maxx){
    lcd.setCursor(15,1);
    lcd.write(8);
    lcd.setCursor(15,0);
    lcd.write(7);  
  }
}

void activateMosfet(){

  //lcd.print("Mosfet ON!");
  digitalWrite(mosfet, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(MOSFET_TIME);   // wait for 4 second
  //lcd.print("Mosfet OFF!");
  digitalWrite(mosfet, LOW);

}
