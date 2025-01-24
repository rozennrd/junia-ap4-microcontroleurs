#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

// Configurer la fréquence d'horloge: 8MHz
#define _XTAL_FREQ 8000000
// Attendre 500ms
 void delai_approx(void) {
        __delay_ms(125);
    }

void main(void) {
    /* Code d'initialisation */

     // Configurer les broches des registres B et D (contrôlant les LEDs) comme sorties
    TRISD = 0x00; 
    TRISB = 0x00;
   
    
    while (1) { // Toujours
        /* Code à exécuter dans une boucle infinie */
        // Eteindre les LEDs  5 à 8 (registre B), allumer successivement les leds 1 à 4 (registre D)
        LATB = 0x00;
        LATD = 0b00000001;//Allumer LED D1
        
        // Attendre 125ms
        delai_approx();
        LATD = 0b00000010;//Allumer LED D2
        
        // Attendre 125ms
        delai_approx();
        LATD = 0b00000100;//Allumer LED D3
        
        // Attendre 125ms
        delai_approx();
        LATD = 0b00001000;//Allumer LED D4
          // Attendre 125ms
        delai_approx();
        // Allumer successivement les LEDs  5 à 8 (registre B), éteindre les leds 1 à 4 (registre D)
        LATD = 0x00;
        LATB = 0b00000001; // Allumer LED D5
        
        // Attendre 125ms
        delai_approx();
        LATB = 0b00000010; // Allumer LED D6
        
        // Attendre 125ms
        delai_approx();
        LATB = 0b00000100; // Allumer LED D7
        
        // Attendre 125ms
        delai_approx();
        LATB = 0b00001000; // Allumer LED D8
        
        delai_approx();
    }
    
   
    
}