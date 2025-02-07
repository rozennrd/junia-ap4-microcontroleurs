#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC
#include "utils.h"      // D�finition des fonctions utiles (setup)

#define _XTAL_FREQ 8000000

void set_PWM4_duty_cycle(uint8_t duty_cycle_percentage) {
    uint16_t duty_cycle = (duty_cycle_percentage * (4 * (PR2 + 1))) / 100;

    PWM4DCH = (duty_cycle >> 2) & 0xFF;       // 8 bits de poids fort
    PWM4DCL = (duty_cycle & 0x03) << 6;       // 2 bits de poids faible dans les bits <7:6>
}

void main(void) {
    /* Code d'initialisation */
    // ON CONFIGURE LES BITS 
    TRISD &= 0xFE; // seule la premi�re led peut �tre allum�e
    TRISB &= 0xFF; // Les leds sur le registre B ne peuvent pas �tre allum�es
    
    RD0PPS = 0b01111;  // On branche D0 sur la valeur de PWM4

    TRISBbits.TRISB0 = 1;       // Configurer RB0 comme entr�e (bouton S1)
                                // Attention: la led D1 sera toujours allum�e quand RB0 =1
    ANSELBbits.ANSB0 = 0;       // D�sactiver le mode analogique sur RB0
    
    PWM4CONbits.PWM4EN = 1; // Activation de PWM4
    setupTimer2();
   
    while (1) {
       if (PORTBbits.RB0 == 0) {          // Bouton S1 appuy� (niveau bas)
            set_PWM4_duty_cycle(100);       // LED D1 � 100% d?intensit�
        } else {                           // Bouton S1 rel�ch� (niveau haut)
            set_PWM4_duty_cycle(10);        // LED D1 � 10% d?intensit�
        }
    }
}

