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
#include "LPD8806.h"	// utilisation des méthodes de la classe LPD8806
#include "SPI.h"		// utilisation des méthodes de la classe SPI
#include "MsTimer2.h"  // pour interruptions du timer 2


const uint16_t NB_LED = 160;  // nombre de LED dans le ruban
const uint8_t LUM = 60;      // luminosité des LED
const int PUISS_0 = 1;    // on considère P < 1 W -> pas de production d'énergie 
int pinData  = 11; 	// signal DATA du bus SPI
int pinClock = 13;   // signal CLOCK du bus SPI
int pinLED = 10;   // led orange sur carte arduino
int pinCode = 8;	// entrée code Manchester (puissance instantanée)
volatile bool int10ms = false;   // demande d'interruption du timer 2
volatile bool int100ms = false;   // demande d'interruption du timer 2
volatile bool int1000ms = false;   // demande d'interruption du timer 2
volatile bool trameErreur = 0;  // detection erreur trame
volatile bool trameRepos = 0;   // detection etat repos, 1 -> prêt à détecter bit start
volatile bool trameStart = 0;   // detection bit start, 1 -> prêt à lire le code
volatile int nivPrec = 0; // niveau précédemment lu lors du dernier passage
volatile int niv0cpt = 0; // compte le nombre de niveau "0" lu successivement
volatile int niv1cpt = 0; // compte le nombre de niveau "1" lu successivement
volatile int puissInst = 0; // compte le nombre de niveau "1" lu successivement

int pinTest = 9;    // sortie TEST
volatile int nivTest = 0;
int pinTest2 = 10;    // sortie TEST2

// Avec une carte Uno classique, il faut utiliser les broches 11 (data) et
// 13 (clock).
LPD8806 strip = LPD8806(NB_LED);  // création d'un objet strip de type LPD8806

void setup() {
  // Initialise la connexion SPI : MSB first, mode 0, clock 2 MHz, RAZ registres LED
  strip.begin();
  strip.show(); // éteint toutes les LED
        // ----- config interruptions -----
  MsTimer2::set(10, intTimer2); // appelle la fonction intTimer2 toutes les 10 ms
  MsTimer2::start();            // activation de l'interruption du timer 2
  pinMode(pinLED, OUTPUT);      // definit pinLed (digital pin) en sortie
  pinMode(pinCode, INPUT);		// définit pinCode en entrée
  pinMode(pinTest, OUTPUT);     // definit la sortie en TEST
  pinMode(pinTest2, OUTPUT);     // definit la sortie en TEST2
         // ----- intialisation de la liaison serie -----
  Serial.begin(115200);		// demarre la liaison serie, vitesse 9600 bauds
}

void loop() 
{
   static bool niveau = HIGH;    // niveau logique applique a la LED
   static uint32_t puissPrec = 0;
   static bool nivTest2;
   static int cptErreur = 0;

   // ----- demande d'interruption du timer 2 (toutes les 10 ms) -----
   if (int10ms == true)
   {
nivTest = !nivTest;
digitalWrite(pinTest, nivTest);
      int10ms = false;    // accuse reception interruption, pret a signaler prochaine interr.
	   trame_ManchesterLire();	// détecte puis lit le contenu de la trame (codage Manchester)
//Serial.print(" puissInst : "); Serial.print(puissInst); Serial.println("    ");
      if (int100ms == true)
      {
         int100ms = false;    // accuse reception interruption, pret a signaler prochaine interr.
         if (puissInst <= PUISS_0)
            rubanCommanderRepos(puissPrec);
      } 
       // ----- demande d'interruption du timer 2 (toutes les 1000 ms) -----
      if (int1000ms == true)
      {
//Serial.print(" 1000 puissInst : "); Serial.print(puissInst); Serial.println("    ");
         int1000ms = false;    // accuse reception interruption, pret a signaler prochaine interr.
         if (trameErreur == 1 && cptErreur > 2)
         {
            puissInst = 0;
            cptErreur = 0;
         }

         if (puissInst > PUISS_0)
            rubanCommanderPuiss(puissInst);
      }
   }
}

// -------------------------------------------------------------------------------------------
// Interruption timer 2 :  Signale une demande d'interruption du timer 2
// -------------------------------------------------------------------------------------------
void intTimer2()
{
   int10ms = true;
   static int cpt10ms = 0;

   cpt10ms++;  // compte le nb de 10ms écoulée
   if (cpt10ms % 10 == 0)
      int100ms = true;
   if (cpt10ms == 100) // détection d'une seconde
   {
      cpt10ms = 0;
      int1000ms = true;
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

//Serial.println("Repos");
   if (puissPrec > PUISS_0)   // si arrêt de production d'énergie
   {
      ledR = 0;
      for(ledR = 0; ledR < NB_LED; ledR++)   // pour éteindre les LED
         strip.setPixelColor(ledR, 0); // charge couleur allumage du point dans registre
      strip.show(); // allume le ruban de LED en fonction des couleurs définies avant
   }
   else
   {
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

   //                             éteint  ,   bleu  ,   vert  ,   jaune ,   rouge ,  blanc
   const uint32_t T_COULEUR[7] = {0x808080, 0x8080FF, 0xFF8080, 0xFFFF80, 0x80FF80, 0xFFFFFF};
   const uint16_t T_PUISS[7] =   {       1,       10,       40,       80,      170,      300};
   const uint16_t T_COEFF[7] =   {       0,      300,      180,       80,       35,       10};
   const uint16_t T_LED_ON[7] =  {       0,       30,       82,      115,      147,      160};

   uint16_t led;
   uint16_t ledSeuil;
   uint32_t couleur;
   double puissCor;  // puissance corrigée

   if (puissInst <= T_PUISS[0])
      ledSeuil = 0;
   else if (puissInst <= T_PUISS[1])
      ledSeuil = puissInst * T_COEFF[1] / 100;
   else if (puissInst <= T_PUISS[2])
      ledSeuil = T_LED_ON[1] + (puissInst -T_PUISS[1]) * T_COEFF[2] / 100;
   else if (puissInst <= T_PUISS[3])
      ledSeuil = T_LED_ON[2] + (puissInst -T_PUISS[2]) * T_COEFF[3] / 100;
   else if (puissInst <= T_PUISS[4])
      ledSeuil = T_LED_ON[3] + (puissInst -T_PUISS[2]) * T_COEFF[4] / 100;
   else if (puissInst <= T_PUISS[5])
      ledSeuil = T_LED_ON[4] + (puissInst -T_PUISS[2]) * T_COEFF[5] / 100;
   else if (puissInst <= T_PUISS[6])
      ledSeuil = T_LED_ON[5] + (puissInst -T_PUISS[2]) * T_COEFF[6] / 100;
   else 
      ledSeuil = T_LED_ON[6];

//Serial.println("LEDs");

//Serial.print("ledSeuil  "); Serial.print(ledSeuil); Serial.println(" "); 

   for(led = 0; led < NB_LED; led++)
   {
      if (led >= ledSeuil)
      {
         if (led <= T_LED_ON[1])
            strip.setPixelColor(led, T_COULEUR[1]); // charge couleur allumage du point dans registre
         else if (led <= T_LED_ON[2])
            strip.setPixelColor(led, T_COULEUR[2]); // charge couleur allumage du point dans registre
         else if (led <= T_LED_ON[3])
            strip.setPixelColor(led, T_COULEUR[3]); // charge couleur allumage du point dans registre
         else if (led <= T_LED_ON[4])
            strip.setPixelColor(led, T_COULEUR[4]); // charge couleur allumage du point dans registre
         else if (led <= T_LED_ON[5])
            strip.setPixelColor(led, T_COULEUR[5]); // charge couleur allumage du point dans registre
         else if (led <= T_LED_ON[6])
            strip.setPixelColor(led, T_COULEUR[6]); // charge couleur allumage du point dans registre
      }
      else
         strip.setPixelColor(led, 0); // charge couleur allumage du point dans registre
   }
   strip.show(); // allume le ruban de LED en fonction des couleurs définies avant

//digitalWrite(pinTest, LOW);
}


// -------------------------------------------------------------------------------------------
// Change l'etat de la LED (basculement)
// niv : variable booleenne contenant l'etat courant de la LED
// sortie : int -> renvoie l'etat inverse de celui recu
// -------------------------------------------------------------------------------------------
int changerEtatLed(boolean niv)
{
   digitalWrite(pinLED, niv); // envoie le niveau logique "niveau" sur la LED
   return !niv;               // renvoie l'autre etat logique
}

// -------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------
void trame_ManchesterLire()
{
   trameReposDetecter();   // détecte état repos, 8 niveaux "1" consécutifs
   if (trameRepos == 1)    // état repos détecté, prêt à lire le code quand il arrivera
         trameCodeManchLire(); // lit code Manchester de la trame
}

// -------------------------------------------------------------------------------------------
// Réinitialise le processus de détection de trame
// -------------------------------------------------------------------------------------------
void trameInit()
{
   trameRepos = 0;
   trameStart = 0;
   niv0cpt = 0;
   niv1cpt = 0;
   nivPrec = 0;
}

// -------------------------------------------------------------------------------------------
// Détecte l'arrivée d'une trame. Une trame est toujours précédée d'un état repos
// Etat repos : état avant émission trame codée en Manchester
//              soit 8 niveaux à "1" consécutifs mini (80 ms mini à "1"), pas de transition 
// trameRepos = 0 -> état repos non détecté à l'entrée code
// trameRepos = 1 -> état repos détecté, la lecture de bit peut commencer (attente bit start)
// -------------------------------------------------------------------------------------------
void trameReposDetecter()
{
   int niv;                // niveau logique lu sur l'entrée code

   niv = digitalRead(pinCode);
   if (niv == 1)
   {
      niv1cpt++;  // compte le nombre de niveau "1" consécutifs
      if (niv1cpt > 7)
      {
         trameRepos = 1;   // ******** état repos détecté *********************************
         niv1cpt = 0;
      }
      if (niv1cpt > 1000)      // !!!!!!!!! ERREUR trame : pas de trame arrivée depuis 5s
      {
         trameErreur = true;  // !!!!!!!!! signale une ERREUR dans la trame
         trameInit();  // initialise le processus pour détecter une nouvelle trame
      }
   }
   else if (trameRepos == 0)
      niv1cpt = 0;
}

// -------------------------------------------------------------------------------------------
// Mémorise le contenu de la trame (sans le bit de start)
// Tous les niveaux logiques sont mémorisés puis analysés pour trouver le code et les 
// éventuelles erreurs
// Codage d'un bit en Manchester
// Vitesse émetteur :  bit = 0 -> 20 ms à l'état "0" puis 20 ms à l'état "1" (front montant)
//                     bit = 1 -> 20 ms à l'état "1" puis 20 ms à l'état "0" (front descendant)
// Lecture récepteur :  lit valeur entrée code toutes les 10 ms
// Il y a 10 bits Manch. à lire puis l'équivalent de 3 bits Manch. (état repos)
//   soit une durée de : 13 * 2 * 20 ms = 520 ms
//   soit 13 * 4 = 52 échantillons soit 1 ech. / 5ms)
// -------------------------------------------------------------------------------------------
void trameCodeManchLire()
{
   static int numEch = 0;    // numero échantillon
   static int tEch[53];  // échantillons mémorisés pour détecter le code de la trame
   unsigned int poids = 256;
   bool erreur = 0;
   unsigned int bitVal;
   unsigned int niv;
   static int puissTemp;

   niv = digitalRead(pinCode);

   if (numEch != 0 || niv == 0) 
   {
      tEch[numEch] = digitalRead(pinCode);
      if (numEch >= 52)
      {
         puissTemp = 0;
         for (int i = 0 ; i < 52 ; i = i + 4)
         {
            if ((tEch[i] == 0) && (tEch[i + 3] == 1))
               bitVal = 0;
            else if ((tEch[i] == 1) && (tEch[i + 3] == 0))
               bitVal = 1;
            else if ((tEch[i] == 1) && (tEch[i + 1] == 1) && (tEch[i + 2] == 1) && (tEch[i + 3] == 1))
               bitVal = 2; // état repos
            else
               bitVal = 0xFFFF;

            if (bitVal == 0xFFFF)
               erreur = true;  // !!!!!!!!! signale une ERREUR dans la trame
            switch (i)
            {
               case 0 :      // 1er bit -> bit start
                  if (bitVal != 0)
                     erreur = true;  // !!!!!!!!! signale une ERREUR dans la trame
                  break;
               case 40 :      // 11ème bit -> bit stop
                  if (bitVal != 1)
                     erreur = true;  // !!!!!!!!! signale une ERREUR dans la trame
                  break;
               case 44 :      // 12ème bit -> état arrêt
               case 48 :      // 13ème bit -> état arrêt
                  if (bitVal != 2)
                     erreur = true;  // !!!!!!!!! signale une ERREUR dans la trame
                  break;
               default :      // 2ème à 10ème bits -> code binaire de la puissance instantanée
                  if (bitVal == 1)
                     puissTemp = puissTemp + poids;
                  poids = poids / 2;
            }

           if (erreur == true)
              break;
//Serial.print(tEch[i]); Serial.print(tEch[i+1]); Serial.print(tEch[i+2]); Serial.print(tEch[i+3]); Serial.print(' ');
         }  
         if (erreur == false)
         {
            trameErreur = false;
            puissInst = puissTemp;
         }
         else
            trameErreur = true;
         trameInit();  // initialise le processus pour détecter une nouvelle trame
         numEch = 0;
//Serial.print(" puissInst : "); Serial.print(puissInst); Serial.print("     ");
//Serial.print(erreur); Serial.print(trameErreur); Serial.println("    ");
      }
      else numEch++;
   }
}