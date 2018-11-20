#ifdef _MSC_VER
	     // Select your target Arduino board here
	     #define __AVR_ATmega168__
	     #define __attribute__(x)
	     #define __inline__
	     #define __cplusplus
	     #define __builtin_va_list int
	     #define __asm__(x)
	     #include "Arduino.h"
#endif

/******************************************************************************
 Chenillard pour tester le ruban de 5 mètres de LED strip adressables (160 LED)
 Commande du ruban de LED basé sur le LPD8806
 Allumage successif de chacune des 160 LED (point RGB) dans l'ordre croissant
 1er passage en rouge, 2ème en vert, 3ème en bleu, 4ème en blanc
******************************************************************************/
#include <LPD8806.h>	// utilisation des méthodes de la classe LPD8806
#include <SPI.h>		// utilisation des méthodes de la classe SPI
#include <MsTimer2.h>  // pour interruptions du timer 2


const uint16_t NB_LED = 160;  // nombre de LED dans le ruban
const uint8_t LUM = 60;      // luminosité des LED
const int PUISS_0 = 2;    // on considère P < 1 W -> pas de production d'énergie 
int pinData  = 51; 	// signal DATA du bus SPI
int pinClock = 52;   // signal CLOCK du bus SPI
int pinLED13 = 13;   // led orange sur carte arduino
volatile bool int10ms = false;   // demande d'interruption du timer 2
volatile bool int100ms = false;   // demande d'interruption du timer 2
volatile bool int300ms = false;   // demande d'interruption du timer 2

int pinTest = 10;    // sortie TEST
int pinTest2 = 12;    // sortie TEST2

// Avec une carte Uno classique, il faut utiliser les broches 11 (data) et
// 13 (clock).
LPD8806 strip = LPD8806(NB_LED);  // création d'un objet strip de type LPD8806

void setup() {
  // Initialise la connexion SPI : MSB first, mode 0, clock 2 MHz, RAZ registres LED
  strip.begin();
  strip.show(); // éteint toutes les LED
        // ----- config interruptions -----
  MsTimer2::set(10, intTimer2); // appelle la fonction intTimer2 toutes les 20 ms
  MsTimer2::start();            // activation de l'interruption du timer 2
  pinMode(pinLED13, OUTPUT);      // definit outLed (digital pin 13) en sortie
  pinMode(pinTest, OUTPUT);     // definit la sortie en TEST
  pinMode(pinTest2, OUTPUT);     // definit la sortie en TEST2
         // ----- intialisation de la liaison serie -----
  Serial.begin(115200);		// demarre la liaison serie, vitesse 9600 bauds
}

void loop() 
{
   static bool niveau = HIGH;    // niveau logique applique a la LED
   uint32_t puissInst, puissAff;
   static uint32_t puissPrec = 0;
   static bool nivTest2;

   // ----- demande d'interruption du timer 2 (toutes les 10 ms) -----
   if (int10ms == true)
   {
      int10ms = false;    // accuse reception interruption, pret a signaler prochaine interr.

      if (int100ms == true)
      {
         int100ms = false;    // accuse reception interruption, pret a signaler prochaine interr.
         puissInst = analogRead(A0);
            // Pmax = 320 -> toutes les LED sont allumées
         puissAff = (puissInst * 161) / 450 - 2; // plage 0 / 320 W
//   Serial.println(puissAff);
         if (puissAff <= PUISS_0)
            rubanCommanderRepos(puissPrec);
         puissPrec = puissAff;
      } 
      // ----- demande d'interruption du timer 2 (toutes les 300 ms) -----
      if (int300ms == true)
      {
         int300ms = false;    // accuse reception interruption, pret a signaler prochaine interr.
         if (puissAff > PUISS_0)
            rubanCommanderPuiss(puissAff);
         niveau = changerEtatLed(niveau); // clignotement LED13
      }
   }
}

// -------------------------------------------------------------------------------------------
// Interruption timer 2 :  Signale une demande d'interruption du timer 2
// -------------------------------------------------------------------------------------------
void intTimer2()
{
   int10ms = true;
   int static cpt10ms = 0;

   cpt10ms++;  // compte le nb de 10ms écoulée
   if (cpt10ms % 10 == 0)
      int100ms = true;
   if (cpt10ms == 30)
   {
      cpt10ms = 0;
      int300ms = true;
   }
}

// Une lumière (point) descend le long du ruban
void rubanCommanderRepos(uint32_t puissPrec) 
{
   // Bornes max définissant les couleurs des LED du ruban en fonction de la puissance 
   // instantanée produite (en W)
   const uint16_t LED_OFF = 3;        // led <= LED_OFF -> LED éteintes
   const uint16_t LED_BLEU = 105; // bleu si < 5 W 
   const uint16_t LED_VERT = 140; // vert si < 50 W 
   const uint16_t LED_JAUNE = 175; // jaune si < 100 W  
   const uint16_t LED_ROUGE = 250; // rouge si < 200 W 
   const uint16_t LED_BLANC = 999; // blanc si >= 200 W 
   static uint16_t ledR = 0;
   uint32_t couleur;

   if (puissPrec > PUISS_0)   // si arrêt de production d'énergie
   {
      ledR = 0;
      for(ledR = 0; ledR < NB_LED; ledR++)   // pour éteindre les LED
         strip.setPixelColor(ledR, 0); // charge couleur allumage du point dans registre
      strip.show(); // allume le ruban de LED en fonction des couleurs définies avant
   }
   else
   {
Serial.println(puissPrec);  
Serial.println(ledR);  
      if (ledR <= LED_OFF)
         couleur = strip.Color(  0,   0,   0);   // LED éteintes, mélange R G B
      else if (ledR <= LED_BLEU / 2)
         couleur = strip.Color(  0,   0, 127);   // Bleu, mélange R G B
      else if (ledR <= LED_VERT / 2)
         couleur = strip.Color(  0, 127,   0);   // Vert, mélange R G B
      else if (ledR <= LED_JAUNE / 2)
         couleur = strip.Color(127, 127,   0);   // Jaune, mélange R G B
      else if (ledR <= LED_ROUGE / 2)
         couleur = strip.Color(127,   0,   0);   // Rouge, mélange R G B
      else if (ledR <= LED_BLANC / 2)
         couleur = strip.Color(127, 127, 127);   // Blanc, mélange R G B
      
      strip.setPixelColor(ledR, couleur); // charge couleur allumage du point dans registre
      strip.show(); // allume le ruban de LED en fonction des couleurs définies avant
      strip.setPixelColor(ledR, 0); // charge couleur allumage du point dans registre
      ledR++;
      if (ledR >= NB_LED)
         ledR = 0;
   }
}

// puiss : puissance instantanée
void rubanCommanderPuiss(uint32_t puiss) 
{
   // Bornes max définissant les couleurs des LED du ruban en fonction de la puissance 
   // instantanée produite (en W)
   // Si P > 0 -> allumage des 100 premières led
   const uint16_t LED_OFF = 3;        // led <= LED_OFF -> LED éteintes
   const uint16_t LED_BLEU = 105; // bleu si < 5 W 
   const uint16_t LED_VERT = 140; // vert si < 50 W 
   const uint16_t LED_JAUNE = 175; // jaune si < 100 W  
   const uint16_t LED_ROUGE = 250; // rouge si < 200 W 
   const uint16_t LED_BLANC = 999; // blanc si >= 200 W 
   uint16_t led;
   uint32_t couleur;
   double puissCor;  // puissance corrigée

digitalWrite(pinTest, HIGH);
// Serial.println(puiss);
   
   puissCor = (double) puiss + 100.0 - ((double) puiss / 4.0);  // allumage des premières LED dès que production d'énergie
   puiss = (uint16_t) puissCor;

   for(led = 0; led < NB_LED; led++)
   {
      if (led <= LED_OFF)
         couleur = strip.Color(  0,   0,   0);   // LED éteintes, mélange R G B
      else if (led <= LED_BLEU / 2)
         couleur = strip.Color(  0,   0, 127);   // Bleu, mélange R G B
      else if (led <= LED_VERT / 2)
         couleur = strip.Color(  0, 127,   0);   // Vert, mélange R G B
      else if (led <= LED_JAUNE / 2)
         couleur = strip.Color(127, 127,   0);   // Jaune, mélange R G B
      else if (led <= LED_ROUGE / 2)
         couleur = strip.Color(127,   0,   0);   // Rouge, mélange R G B
      else if (led <= LED_BLANC / 2)
         couleur = strip.Color(127, 127, 127);   // Blanc, mélange R G B
      
      if (puiss / 2 >= led)
         strip.setPixelColor(led, couleur); // charge couleur allumage du point dans registre
      else
         strip.setPixelColor(led, 0); // charge couleur allumage du point dans registre
   }
   strip.show(); // allume le ruban de LED en fonction des couleurs définies avant

digitalWrite(pinTest, LOW);
}


// -------------------------------------------------------------------------------------------
// Change l'etat de la LED (basculement)
// niv : variable booleenne contenant l'etat courant de la LED
// sortie : int -> renvoie l'etat inverse de celui recu
// -------------------------------------------------------------------------------------------
int changerEtatLed(boolean niv)
{
   digitalWrite(pinLED13, niv); // envoie le niveau logique "niveau" sur la LED
   return !niv;               // renvoie l'autre etat logique
}