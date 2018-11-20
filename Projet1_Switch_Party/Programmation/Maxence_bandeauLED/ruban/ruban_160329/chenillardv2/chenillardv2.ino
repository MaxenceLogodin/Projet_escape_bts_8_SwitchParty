/******************************************************************************
 Chenillard pour tester le ruban de 5 mètres de LED strip adressables (160 LED)
 Commande du ruban de LED basé sur le LPD8806
 Allumage successif de chacune des 160 LED (point RGB) dans l'ordre croissant
 1er passage en rouge, 2ème en vert, 3ème en bleu, 4ème en blanc
******************************************************************************/
#include "SPI.h"    // utilisation des méthodes de la classe SPI
#include "LPD8806.h"	// utilisation des méthodes de la classe LPD8806


const int DUREE_ON = 25;  // durée allumage de chaque LED
int nLEDs = 160; 	   // nombre de LED RGB dans le ruban
int dataPin  = 51; 	// signal DATA du bus SPI
int clockPin = 52;   // signal CLOCK du bus SPI

// Avec une carte Uno classique, il faut utiliser les broches 11 (data) et
// 13 (clock).
LPD8806 strip = LPD8806(nLEDs);  // création d'un objet strip de type LPD8806

void setup() {
  // Initialise la connexion SPI : MSB first, mode 0, clock 2 MHz, RAZ registres LED
  strip.begin();

  // envoi (n registres LED -> n PWM LED), PWM = 0 -> n LED éteintes 
  strip.show();
}

void loop() {
  colorChase(strip.Color(127,  0,  0), DUREE_ON);  // chenillard, rouge défile
  colorChase(strip.Color(  0,127,  0), DUREE_ON);  // chenillard, vert défile
  colorChase(strip.Color(  0,  0,127), DUREE_ON);  // chenillard, bleu défile
  colorChase(strip.Color(127,127,127), DUREE_ON);  // chenillard, blanc défile
}

// Une lumière (point) descend le long du ruban
// c : couleur de la lumière,  wait : durée d'allumage
void colorChase(uint32_t c, uint8_t wait) {
  int i;
  
  // un seul point est affiché à la fois
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // charge couleur allumage du point dans registre
    strip.show();              // allume le point, registre LED -> PWM LED 
    strip.setPixelColor(i, 0); // RAZ registre LED, mais PWM tjs active
    delay(wait);               // temporisation
  }

  strip.show(); // Eteint le dernier point
}







