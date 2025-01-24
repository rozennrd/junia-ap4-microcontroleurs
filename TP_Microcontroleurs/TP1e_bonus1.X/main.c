#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

/* TP1 exercice 3 : idem exercice 2 mais en utilisant les interruptions. Cela permet de gagner en fiabilité et en précision */


// Configurer la fréquence d'horloge: 8MHz
#define _XTAL_FREQ 8000000

// Variables globales
volatile uint8_t ledState = 0b00000001; // Démarrer avec D1 active


// Attendre 125ms : cela permet de comparer la vitesse d'exécution avec le demi-chenillard qui utilise le timer
void delai_approx(void) {
    __delay_ms(125);
}


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
        
        // Décaler l'état des LEDs
        ledState <<= 1; // Décale à gauche
        if (ledState == 0b00010000) {
            ledState = 0b00000001; // Revenir à D1 après D4
        }
    }
}

void main(void) {
    /* Code d'initialisation */

     // Configurer les broches des registres B et D (contrôlant les LEDs) comme sorties
    TRISD = 0x00; 
    TRISB = 0x00;
    // Configuration du Timer2
    setupTimer2();
    
     while (1) {
       /* Code à exécuter dans une boucle infinie */
        // Allumer LED D8
        LATB = 0b00001000;
        
        // Attendre 125 ms
        delai_approx();
        //  Allumer LED D7
        LATB = 0b00000100;
        
         // Attendre 125 ms
        delai_approx();
        //  Allumer LED D6
        LATB = 0b00000010;
         // Attendre 125 ms
        delai_approx();
        // Allumer led D5
        LATB = 0b00000001;
        
        // Attendre 125 ms
         delai_approx();
    }
      
    
    /* Commentaire sur l'utilité de l'interruption :
     * Les LEDs D1 à D4 défilent plus vite que les leds D5 à D8. Le timer et les interruption permettent
     * de gagner en fiabilité et en précision, et d'atteindre effectivement la demi seconde pour le 
     * demi chenillard (soit un chenillard d'une seconde). A contrario, le demi chenillard D8-D5 est 
     * plus lent et son déroulement est moins réactif : plus long qu'une demi seconde 
     */
}