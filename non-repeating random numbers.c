#include <LiquidCrystal.h>

// LiquidCrystal lcd(rs, enable, d4, d5, d6, d7) 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

String shuffleDezenas, sortDezenas, dezenas;
byte maxSorteio = 6, maxDezenas = 60;

byte botoes = 5;
int estadoBotoes = 0;

#define btnRight 1
#define btnLeft 2
#define btnUp 3
#define btnDown 4
#define btnSelect 5
#define btnNone 6


void setup() {

  byte k;

  randomSeed(analogRead(1));

  lcd.begin(16, 2);

  lcd.clear();

  tela();

  criarDezenas();

}


void loop() {

  byte k;

  botoes = ler_botoes();

  if (botoes == btnRight) {
    maxSorteio = constrain(maxSorteio + 1, 1, 10);
    tela();
  }

  if (botoes == btnLeft) {
    maxSorteio = constrain(maxSorteio - 1, 1, 10);
    tela();
  }

  if (botoes == btnUp) {
    maxDezenas = constrain(maxDezenas + 1, 10, 99);
    criarDezenas();
    tela();
  }

  if (botoes == btnDown) {
    maxDezenas = constrain(maxDezenas - 1, 10, 99);
    criarDezenas();
    tela();
  }

  if (botoes == btnSelect) {

    lcd.clear();

    for (byte rep = 1; rep <= 9; rep++) {

      shuffle();

      delay(100);

      lcd.setCursor(0, 0);

      for (k = 1; k <= maxSorteio / 2; k++) {
        lcd.print(shuffleDezenas.substring((k - 1) * 2, (k - 1) * 2 + 2));
        lcd.print(" ");
      }

      lcd.setCursor(0, 1);

      for (k = maxSorteio / 2 + 1; k <= maxSorteio; k++) {
        lcd.print(shuffleDezenas.substring((k - 1) * 2, (k - 1) * 2 + 2));
        lcd.print(" ");
      }

    }

    sort();

    lcd.setCursor(0, 0);

    for (k = 1; k <= maxSorteio / 2; k++) {
      lcd.print(sortDezenas.substring((k - 1) * 2, (k - 1) * 2 + 2));
      lcd.print(" ");
    }

    lcd.setCursor(0, 1);

    for (k = maxSorteio / 2 + 1; k <= maxSorteio; k++) {
      lcd.print(sortDezenas.substring((k - 1) * 2, (k - 1) * 2 + 2));
      lcd.print(" ");
    }

  }

}


int ler_botoes() {           // Buttons reading

  delay(120);

  estadoBotoes = analogRead(0);

  if (estadoBotoes > 1000) return btnNone;
  if (estadoBotoes < 50)   return btnRight;
  if (estadoBotoes < 250)  return btnUp;
  if (estadoBotoes < 450)  return btnDown;
  if (estadoBotoes < 650)  return btnLeft;
  if (estadoBotoes < 850)  return btnSelect;

  return btnNone;           // Return NONE when all options fails

}


void shuffle() {            // Shuffling routine

  byte k, n;
  String tempSeq = dezenas;

  shuffleDezenas = "";

  for (k = 0; k < maxSorteio; k++) {
    n = random(tempSeq.length() / 2);
    shuffleDezenas = shuffleDezenas + tempSeq.substring(n * 2 , n * 2 + 2);
    tempSeq.remove(n * 2, 2);
  }

}


void sort() {                 // Sorting routine

  String tempSeq, c;
  byte i, k, n, menorDezena, menorPos;

  sortDezenas = "";
  tempSeq = shuffleDezenas;
  menorDezena = maxDezenas;

  do {
    for (k = 0; k < tempSeq.length() / 2; k++) {
      n = (tempSeq.substring(k * 2 , k * 2 + 2)).toInt();
      if (n <= menorDezena) {
        menorDezena = n;
        menorPos = k;
        c = tempSeq.substring(k * 2 , k * 2 + 2);
      }
    }

    tempSeq.remove(menorPos * 2, 2);
    sortDezenas = sortDezenas + c;
    menorDezena = maxDezenas;
  }  while (tempSeq.length() > 0);

}


void criarDezenas() {         // Numbers list creation routine

  byte k;

  dezenas = "";

  for (k = 1; k <= maxDezenas; k++) {
    if (k < 10) {
      dezenas = dezenas + "0" + k;
    }
    else {
      dezenas = dezenas + k;
    }
  }

}


void tela() {                 // LCD print routine

  lcd.setCursor(0, 0);
  lcd.print("LOTTERY|");
  lcd.setCursor(0, 1);
  lcd.print("WINNER |");
  lcd.setCursor(8, 0);
  lcd.print("TOT|DRAW");
  lcd.setCursor(9, 1);
  lcd.print(maxDezenas);
  lcd.print("| ");
  if (maxSorteio < 10) {
    lcd.print("0");
    lcd.print(maxSorteio);
  }
  else {
    lcd.print(maxSorteio);
  }

}
