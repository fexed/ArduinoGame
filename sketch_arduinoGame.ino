#include <LiquidCrystal.h>
#include <math.h>
#include "pitches.h"

LiquidCrystal lcd(13, 12, 5, 4, 3, 2);
const int dbtn = 7;
const int ubtn = 8; //pin pulsanti
const int sound = 6; //pin audio
const int okbeep = 262;
const int nobeep = 196;
const int duratabeep = 50;
int intro = 1;
int dbtnstate = 0;
int ubtnstate = 0; //stato pulsanti
int character = 0; //riga personaggio
int scroll = 0; //variabile per controllo velocità ostacoli
int pos[] = {14, 14}; //colonne ostacoli
int col[] = {0, 1}; //righe ostacoli
int vela = 3; //velocità primo ostacolo
int velb = 5; //velocità secondo ostacolo
int pnta = 0; //unità punteggio
int pntb = 0; //decine
int pntbp = 0;
int colpiti = 0;

int intromelody[] = {
  262, 196, 196, 220, 196, 0, 247, 262
};
int gameovermelody[] = {
  15, NOTE_C4, 8, NOTE_H, 8, NOTE_H, 8, NOTE_G3, 8, NOTE_H, 4, NOTE_E3, 4, NOTE_A3, 6, NOTE_B3, 6, NOTE_A3, 6, NOTE_GS3, 6, NOTE_AS3, 6, NOTE_GS3, 6, NOTE_G3, 8, NOTE_F3, 8, NOTE_G3, 4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  lcd.begin(16, 2); //preparazione lcd
  lcd.noBlink();
  pinMode(ubtn, INPUT); //pulsante up
  pinMode(dbtn, INPUT); //pulsante down
  //pinMode(sound, OUTPUT); //audio
}

void loop() {
  lcd.clear();
  if (intro) {
    intro = 0;
    lcd.print("Obstacle      by");
    lcd.setCursor(0, 1);
    lcd.print("Race       Xedef");
    for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(sound, intromelody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(sound);
  }
  
  } else {
    if (colpiti == 5) {
      lcd.print("Game        Over");
      for (int thisNote = 1; thisNote < (gameovermelody[0] * 2 + 1); thisNote = thisNote + 2) { // Run through the notes one at a time
        tone(sound, gameovermelody[thisNote], (1000/gameovermelody[thisNote + 1]));// Play the single note
        delay((1000/gameovermelody[thisNote + 1]) * 1.30);                        // Delay for the specified time
        noTone(sound);                                                 // Silence the note that was playing
      }
      colpiti = 0;
      intro = 1;
      delay(3000);
    } else {
      lcd.setCursor(15,0);
      if (pntb > 0) lcd.print(pntb);
      lcd.setCursor(15,1);
      lcd.print(pnta); //stampa del punteggio (decine sopra e unità sotto)
      scroll = (scroll + 1) % 100; //incremento variabile scroll per controllo velocità ostacoli
      
      if (!(scroll % vela)) pos[0]--;
      if (pos[0] < 0) {
        if (col[0] != character) {pnta++; tone(sound, okbeep, duratabeep);}
        else {tone(sound, nobeep, duratabeep); colpiti++;}
        pos[0] = 14;
        col[0] = rand() % 2;
        vela = (rand() % 5) + 1;
      }
      lcd.setCursor(pos[0], col[0]);
      lcd.write(1023);
      
      if (!(scroll % velb)) pos[1]--;
      if (pos[1] < 0) {
        if (col[1] != character) {pnta++; tone(sound, okbeep, duratabeep);}
        else {tone(sound, nobeep, duratabeep); colpiti++;}
        pos[1] = 14;
        col[1] = rand() % 2;
        velb = (rand() % 6) + 1;
      }
      lcd.setCursor(pos[1], col[1]);
      lcd.write(1023);
    
      if (pnta == 10) {pnta = 0; pntb++;}
      dbtnstate = digitalRead(dbtn);
      ubtnstate = digitalRead(ubtn);
      if (dbtnstate == HIGH) character = 0;
      else if (ubtnstate == HIGH) character = 1;
      lcd.setCursor(0, character);
      lcd.print("O");
      if (pntb > pntbp) {
        pntbp = pntb;
        noTone(sound);
        tone(sound, 500, 25);
      }
      delay(50);
    }
  }
}
