#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

// Configurer la fréquence d'horloge: 8MHz
#define _XTAL_FREQ 8000000

// Attendre 500ms
 void delai_approx(void) {
        __delay_ms(500);
    }

void main(void) {
    /* Code d'initialisation */

     // Configurer les broches des registres B et D (contrôlant les LEDs) comme sorties
    TRISD &= 0xF0; 
    TRISB &= 0xF0;
   
    
    while (1) { // Toujours
        /* Code à exécuter dans une boucle infinie */
        // Allumer les LEDs  5 à 8 (registre B), éteindre les leds 1 à 4 (registre D)
        LATD = 0x00;
        LATB = 0x0F;
        
        // Attendre 500ms
        delai_approx();
        
        // Éteindre les LEDs  5 à 8 (registre B), allumer les leds 1 à 4 (registre D)
        LATB = 0x00;
        LATD = 0x0F; 
        // Attendre 500 ms. Le temps total approximatif de la boucle est 1s. 
        delai_approx();
    }
    
   
    
}

