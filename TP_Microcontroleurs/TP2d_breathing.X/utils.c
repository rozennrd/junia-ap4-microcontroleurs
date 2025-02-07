#include "configbits.h" // Bits de configuration
#include <xc.h> 
void setupTimer2(){
    // Fonction pour configurer Timer2
    T2CONbits.T2CKPS = 0b11;  // Prescaler = 1:1
    T2CONbits.T2OUTPS = 0b1111; // Postscaler = 1:8
    PR2 = 250;                // Période de Timer2 pour 1ms
    T2CONbits.TMR2ON = 1;     // Activer Timer2
    
    /* Note : PIR1 = contient les flags d'interruption ; indique si une interruption a eu lieu et sur quel périphérique
     PIE1bits active ou désactive les interruptions des périphériques (= est-ce que en cas d'interruption on écrit cette interruption dans PIR1)
     */
    // SETUP interruptions
    /*PIR1bits.TMR2IF = 0;       // Effacer le flag d'interruption Timer2
    PIE1bits.TMR2IE = 1;       // Activer l'interruption Timer2
    INTCONbits.PEIE = 1;       // Activer les interruptions périphériques
    INTCONbits.GIE = 1;        // Activer les interruptions globales*/
}


void interrupt_timer_with_S2() {
  TRISAbits.TRISA5 = 1; // Le bit 5 du registre A est en entrée
  ANSELAbits.ANSA5 = 0; // Désactive le mode analogique

  while (1) {

    if (PORTAbits.RA5 ==
        0) { // Lecture de l'entrée numérique (lorsque le bouton est pressé)
      if (T2CONbits.TMR2ON == 1) { // Si le timer est lancé
        T2CONbits.TMR2ON = 0; // Arrête Timer2, le défilement des leds s'arrête.

      } else {                // Sinon
        T2CONbits.TMR2ON = 1; // Relance Timer2, le défilement des leds reprend.
      }
    }
  }
}
