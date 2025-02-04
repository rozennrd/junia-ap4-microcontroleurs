#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

// Configurer la fr�quence d'horloge: 8MHz
#define _XTAL_FREQ 8000000

// Variables globales
volatile uint8_t ledState = 0b00000001; // D�marrer avec D1 active
volatile uint8_t clockCnt = 0;

// Fonction pour configurer Timer2
void setupTimer2(void) {
    T2CONbits.T2CKPS = 0b0;  // Prescaler = 1:1
    T2CONbits.T2OUTPS = 0b1000; // Postscaler = 1:8
    PR2 = 249;                // P�riode de Timer2 pour 1ms
    T2CONbits.TMR2ON = 1;     // Activer Timer2
}


void main(void) {
    /* Code d'initialisation */

     // Configurer les broches des registres B et D (contr�lant les LEDs) comme sorties
    TRISD &= 0xF0; 
    TRISB &= 0xF0;
    // Configuration du Timer2
    setupTimer2();
    
     while (1) {
        if (PIR1bits.TMR2IF) { // V�rifier si Timer2 a atteint la p�riode
            PIR1bits.TMR2IF = 0; // R�initialiser le flag TMR2IF
            clockCnt++;
            if (clockCnt == 125) {
                // Mettre � jour les LEDs
                LATD = ledState & 0x0F; // �tat des 4 bits bas (D1 � D4) et croiser avec 0x0F pour n'allumer que la bonne led
                LATB = (ledState >> 4) & 0x0F; // �tat des 4 bits hauts (D5 � D8) : d�calage des 4 bits hauts de ledstate pour correspondre aux 4 bits bas de LATB

                // D�caler l'�tat des LEDs
                ledState <<= 1; // D�caler � gauche
                if (ledState == 0) { // Si aucune led n'est activ�e
                    ledState = 0b00000001; // Recommencer � D1 apr�s D8
                }
                clockCnt = 0;
            }
            
        }
    }
   
   
    
}