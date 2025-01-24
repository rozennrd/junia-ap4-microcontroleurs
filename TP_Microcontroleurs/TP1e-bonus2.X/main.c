#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

// Configurer la fréquence d'horloge: 8MHz
#define _XTAL_FREQ 8000000

// Variables globales
volatile uint8_t ledState = 0b00000001; // Démarrer avec D1 active
volatile uint8_t timerRunning = 1;      // État du Timer2 (1 = en marche, 0 = arrêté)

// Fonction pour configurer Timer2
void setupTimer2(void) {
    T2CONbits.T2CKPS = 0b11;  // Prescaler = 1:16
    T2CONbits.T2OUTPS = 0b1111; // Postscaler = 1:16
    PR2 = 125;                // Période de Timer2 pour 125ms (125ms * 8leds = 1s)
    T2CONbits.TMR2ON = 1;     // Activer Timer2
    
    
    /* Note : PIR1 = contient les flags d'interruption ; indique si une interruption a eu lieu et sur quel périphérique
     PIE1bits active ou désactive les interruptions des périphériques (= est-ce que en cas d'interruption on écrit cette interruption dans PIR1)
     */
    // SETUP interruptions
    PIR1bits.TMR2IF = 0;       // Effacer le flag d'interruption Timer2
    PIE1bits.TMR2IE = 1;       // Activer l'interruption Timer2
    INTCONbits.PEIE = 1;       // Activer les interruptions périphériques
    INTCONbits.GIE = 1;        // Activer les interruptions globales
}

// Routine d'interruption
/* Notes : ISR = Interrupt service routine  - on définit notre routine d'interruption */
void __interrupt() ISR() {
    if (PIR1bits.TMR2IF) {  // Vérifie si l'interruption est déclenchée par Timer2
        PIR1bits.TMR2IF = 0; // Efface le flag d'interruption

        // Mettre à jour les LEDs
        LATD = ledState & 0x0F; // État des 4 bits bas (D1 à D4)
        LATB = (ledState >> 4) & 0x0F; // État des 4 bits hauts (D5 à D8) (décalé pour correspondre à LATB)

        // Décaler l'état des LEDs
        ledState <<= 1; // Décale à gauche
        if (ledState == 0) {
            ledState = 0b00000001; // Revenir à D1 après D8
        }
    }
}

void main(void) {
    /* Code d'initialisation */

     // Configurer les broches des registres B et D (contrôlant les LEDs) comme sorties
    TRISD = 0x00; 
    TRISB = 0x00;            // Configurer RB0 (S2) comme entrée, autres en sortie
    // Configuration du Timer2
    setupTimer2();
    TRISAbits.TRISA5 = 1; // Le bit 5 du registre A est en entrée
    ANSELAbits.ANSA5 = 0; // Désactive le mode analogique
            
     while (1) {

         if(PORTAbits.RA5 == 0) {  // Lecture de l'entrée numérique (lorsque le bouton est pressé)
            if (T2CONbits.TMR2ON == 1) { // Si le timer est lancé
                T2CONbits.TMR2ON = 0; // Arrête Timer2, le défilement des leds s'arrête.

            } else { // Sinon
                T2CONbits.TMR2ON = 1; // Relance Timer2, le défilement des leds reprend.
            }
         }
     }
}