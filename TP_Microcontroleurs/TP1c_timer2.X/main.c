#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

// Configurer la fréquence d'horloge: 8MHz
#define _XTAL_FREQ 8000000

// Variables globales
volatile uint8_t ledState = 0b00000001; // Démarrer avec D1 active
volatile uint8_t clockCnt = 0;

// Fonction pour configurer Timer2
void setupTimer2(void) {
    T2CONbits.T2CKPS = 0b0;  // Prescaler = 1:1
    T2CONbits.T2OUTPS = 0b1000; // Postscaler = 1:8
    PR2 = 249;                // Période de Timer2 pour 1ms
    T2CONbits.TMR2ON = 1;     // Activer Timer2
}


void main(void) {
    /* Code d'initialisation */

     // Configurer les broches des registres B et D (contrôlant les LEDs) comme sorties
    TRISD &= 0xF0; 
    TRISB &= 0xF0;
    // Configuration du Timer2
    setupTimer2();
    
     while (1) {
        if (PIR1bits.TMR2IF) { // Vérifier si Timer2 a atteint la période
            PIR1bits.TMR2IF = 0; // Réinitialiser le flag TMR2IF
            clockCnt++;
            if (clockCnt == 125) {
                // Mettre à jour les LEDs
                LATD = ledState & 0x0F; // État des 4 bits bas (D1 à D4) et croiser avec 0x0F pour n'allumer que la bonne led
                LATB = (ledState >> 4) & 0x0F; // État des 4 bits hauts (D5 à D8) : décalage des 4 bits hauts de ledstate pour correspondre aux 4 bits bas de LATB

                // Décaler l'état des LEDs
                ledState <<= 1; // Décaler à gauche
                if (ledState == 0) { // Si aucune led n'est activée
                    ledState = 0b00000001; // Recommencer à D1 après D8
                }
                clockCnt = 0;
            }
            
        }
    }
   
   
    
}