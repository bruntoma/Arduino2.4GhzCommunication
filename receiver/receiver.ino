// nRF24L01 přijímač

// připojení knihoven
#include <Servo.h>
#include <SPI.h>
#include "RF24.h"
// nastavení propojovacích pinů
#define CE 7
#define CS 8
// inicializace nRF s piny CE a CS
RF24 nRF(CE, CS);
Servo myservo;

int servoPos = 1500;
// nastavení adres pro přijímač a vysílač,
// musí být nastaveny stejně v obou programech!
byte adresaPrijimac[]= "prijimac00";
byte adresaVysilac[]= "vysilac00";

void setup() {
  myservo.attach(2);
  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(2000000);
  // zapnutí komunikace nRF modulu
  nRF.begin();
  // nastavení výkonu nRF modulu,
  // možnosti jsou RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX,
  // pro HIGH a MAX je nutný externí 3,3V zdroj
  nRF.setPALevel(RF24_PA_LOW);
  // nastavení zapisovacího a čtecího kanálu
  nRF.openWritingPipe(adresaPrijimac);
  nRF.openReadingPipe(1,adresaVysilac);
  // začátek příjmu dat
  nRF.startListening();
}

unsigned long lastTime = 0;
unsigned long count = 0;
unsigned long period = 0;
char prijem[8];
unsigned long odezva;

void loop() {
  // proměnné pro příjem a odezvu

  if (millis() - lastTime >= 1000)
  {
    //Serial.println("ASDASDASD");
    lastTime = millis();
    period = count;
    count = 0;
  }
  
  // v případě, že nRF je připojené a detekuje
  // příchozí data, začni s příjmem dat
  if( nRF.available()){
    // čekání na příjem dat
    while (nRF.available()) {
      // v případě příjmu dat se provede zápis
      // do proměnné prijem
      nRF.read( &prijem, sizeof(char) * 8 );
      servoPos = 100.0/prijem[0];
      count++;
      Serial.println(servoPos);
      if (servoPos > 0)
        myservo.write(2000);
      else if (servoPos < 0)
        myservo.write(1000);
      else
        myservo.writeMicroseconds(1500);
    }
    // vytisknutí přijatých dat na sériovou linku
    Serial.print("Prijata volba: ");
    Serial.print((int)prijem[0]);
    Serial.print(",");
    Serial.print((int)prijem[1]);
    Serial.print(",");
    Serial.print((int)prijem[2]);

    Serial.print(",  Frequency: ");

    Serial.print(period);
    Serial.println();

   
    delay(50);
   // nRF.write( &odezva, sizeof(odezva) );     
    
  }
}