#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

/* TP1 exercice 3 : idem exercice 2 mais en utilisant les interruptions. Cela permet de gagner en fiabilit� et en pr�cision */


// Configurer la fr�quence d'horloge: 8MHz
#define _XTAL_FREQ 8000000

// Variables globales
volatile uint8_t ledState = 0b00000001; // D�marrer avec D1 active


// Fonction pour configurer Timer2
void setupTimer2(void) {
    T2CONbits.T2CKPS = 0b11;  // Prescaler = 1:16
    T2CONbits.T2OUTPS = 0b1111; // Postscaler = 1:16
    PR2 = 125;                // P�riode de Timer2 pour 125ms (125ms * 8leds = 1s)
    T2CONbits.TMR2ON = 1;     // Activer Timer2
    
    
    /* Note : PIR1 = contient les flags d'interruption ; indique si une interruption a eu lieu et sur quel p�riph�rique
     PIE1bits active ou d�sactive les interruptions des p�riph�riques (= est-ce que en cas d'interruption on �crit cette interruption dans PIR1)
     */
    // SETUP interruptions
    PIR1bits.TMR2IF = 0;       // Effacer le flag d'interruption Timer2
    PIE1bits.TMR2IE = 1;       // Activer l'interruption Timer2
    INTCONbits.PEIE = 1;       // Activer les interruptions p�riph�riques
    INTCONbits.GIE = 1;        // Activer les interruptions globales
}

// Routine d'interruption
/* Notes : ISR = Interrupt service routine  - on d�finit notre routine d'interruption */
void __interrupt() ISR() {
    if (PIR1bits.TMR2IF) {  // V�rifie si l'interruption est d�clench�e par Timer2
        PIR1bits.TMR2IF = 0; // Efface le flag d'interruption

        // Mettre � jour les LEDs
        LATD = ledState & 0x0F; // �tat des 4 bits bas (D1 � D4)
        LATB = (ledState >> 4) & 0x0F; // �tat des 4 bits hauts (D5 � D8) (d�cal� pour correspondre � LATB)

        // D�caler l'�tat des LEDs
        ledState <<= 1; // D�cale � gauche
        if (ledState == 0) {
            ledState = 0b00000001; // Revenir � D1 apr�s D8
        }
    }
}

void main(void) {
    /* Code d'initialisation */

     // Configurer les broches des registres B et D (contr�lant les LEDs) comme sorties
    TRISD = 0x00; 
    TRISB = 0x00;
    // Configuration du Timer2
    setupTimer2();
    
     while (1) {
       // While 1 = le programme tourne, mais la logique est contenue dans la routine d'interruption d�finie plus t�t.
    }
   
   
    
}