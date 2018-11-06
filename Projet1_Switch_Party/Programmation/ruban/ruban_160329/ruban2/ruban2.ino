#include "LPD8806.h"
#include "SPI.h"

// Simple test for 160 (5 meters) of LPD8806-based RGB LED strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 160;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 51;
int clockPin = 52;

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
//LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.

// Creation d'un objet strip
LPD8806 strip = LPD8806(nLEDs);

void setup() {
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
}

void loop() {
   unsigned long couleurPWM;
   int pix;
   
   // fabrication bleu avec G, R et B : bleu -> 0x8080FF
   couleurPWM = strip.Color(  0,  0, 127); // Blue
  
   // Envoi en série de tous les codes couleurs pour chaque LED (3 octets / LED)
   for(pix = 0; pix < strip.numPixels(); pix++)
   {
     strip.setPixelColor(pix, couleurPWM);  // Set new pixel 'on'
     strip.show();                          // Refresh LED states
//     strip.setPixelColor(pix, 0);           // Erase pixel, but don't refresh!
     delay(100);
   }

   // Envoi des valeurs des registres PWM sur les sorties LED
   strip.setPixelColor(pix, 0); // MSB à 0 pour 3 octets 
   strip.show(); // registre -> PWM (envoi sur sortie LED)
   
   // attends avant la prochaine séquence
   // je sais qu'au prochain tour ce sera encore tout bleu
   delay(1000);
}
