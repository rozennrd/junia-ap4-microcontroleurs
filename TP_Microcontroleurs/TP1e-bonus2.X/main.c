#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

// Configurer la fr�quence d'horloge: 8MHz
#define _XTAL_FREQ 8000000

// Variables globales
volatile uint8_t ledState = 0b00000001; // D�marrer avec D1 active
volatile uint8_t timerRunning = 1;      // �tat du Timer2 (1 = en marche, 0 = arr�t�)

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
    TRISB = 0x00;            // Configurer RB0 (S2) comme entr�e, autres en sortie
    // Configuration du Timer2
    setupTimer2();
    TRISAbits.TRISA5 = 1; // Le bit 5 du registre A est en entr�e
    ANSELAbits.ANSA5 = 0; // D�sactive le mode analogique
            
     while (1) {

         if(PORTAbits.RA5 == 0) {  // Lecture de l'entr�e num�rique (lorsque le bouton est press�)
            if (T2CONbits.TMR2ON == 1) { // Si le timer est lanc�
                T2CONbits.TMR2ON = 0; // Arr�te Timer2, le d�filement des leds s'arr�te.

            } else { // Sinon
                T2CONbits.TMR2ON = 1; // Relance Timer2, le d�filement des leds reprend.
            }
         }
     }
}