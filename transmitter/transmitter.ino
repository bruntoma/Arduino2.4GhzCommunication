// nRF24L01 vysílač

#define pinX A0
#define pinY A1
#define pinKey 2

int nulaX, nulaY;

// připojení knihoven
#include <SPI.h>
#include "RF24.h"
// nastavení propojovacích pinů
#define CE 7
#define CS 8
// inicializace nRF s piny CE a CS
RF24 nRF(CE, CS);
// nastavení adres pro přijímač a vysílač,
// musí být nastaveny stejně v obou programech!
byte adresaPrijimac[]= "prijimac00";
byte adresaVysilac[]= "vysilac00";

void setup() {
  // inicializace tlačítka joysticku
  pinMode(pinKey, INPUT_PULLUP);
  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(9600);
  // načtení a uložení hodnot pro x a y osy
  nulaX = analogRead(pinX);
  nulaY = analogRead(pinY);


  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(2000000);
  // zapnutí komunikace nRF modulu
  nRF.begin();
  // nastavení výkonu nRF modulu,
  // možnosti jsou RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX,
  // pro HIGH a MAX je nutný externí 3,3V zdroj
  nRF.setPALevel(RF24_PA_LOW);
  // nastavení zapisovacího a čtecího kanálu
  nRF.openWritingPipe(adresaVysilac);
  //nRF.openReadingPipe(1,adresaPrijimac);
  // začátek příjmu dat
  //nRF.startListening();
  nRF.stopListening();
}

void loop() {
   // vytvoření proměnných pro uložení
  // hodnot pro osy x, y a stav tlačítka
  int aktX, aktY, stavTlac;
  // načtení analogových hodnot osy x a y
  aktX = analogRead(pinX) - nulaX;
  aktY = analogRead(pinY) - nulaY;
  // načtení stavu tlačítka
  //stavTlac = digitalRead(pinKey);
  if (aktX > 0) {
    aktX = map(aktX, 0, 1023-nulaX, 0, 100);
  }
  else {
    aktX = map(aktX, 0, -nulaX, 0, -100);
  }
  if (aktY > 0) {
    aktY = map(aktY, 0, 1023-nulaY, 0, 100);
  }
  else {
    aktY = map(aktY, 0, -nulaY, 0, -100);
  }

  char s[8]; 

  s[0] = aktX;
  s[1] = aktY;
 
    // vytisknutí aktuální volby po sériové lince
    Serial.print("Posilam volbu ");
    Serial.print((int)s[0]);
    Serial.print(", ");
    Serial.print((int)s[1]);
    Serial.println();
    // uložení startovního času komunikace
    unsigned long casZacatek = micros();
    // odeslání aktuální volby, v případě selhání
    // vytištění chybové hlášky na sériovou linku
    if (!nRF.write( s, sizeof(char) * 8 )){
       Serial.println("Chyba při odeslání!");
    }

}