#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC
#include "utils.h"      // Définition des fonctions utiles (setup)

#define _XTAL_FREQ 8000000


unsigned int ADC_Read() {
    ADCON0bits.GO = 1; // Commencer la conversion
    while (ADCON0bits.GO); // Attendre la fin de la conversion 
    return ((ADRESH << 8) + ADRESL); // Retourner le résultat (10 bits)
}

void set_PWM4_duty_cycle(long int duty_cycle_percentage) {
    uint16_t duty_cycle = (duty_cycle_percentage * (4 * (PR2 + 1))) / 100;

    PWM4DCH = (duty_cycle >> 2) & 0xFF;       // 8 bits de poids fort
    PWM4DCL = (duty_cycle & 0x03) << 6;       // 2 bits de poids faible dans les bits <7:6>
}


void main(void) {
    /* Code d'initialisation */
    // ON CONFIGURE LES BITS 
    // Entrée potentiomètre = entrée analogue sur RA0
    TRISAbits.TRISA0 = 1;        // RA0 = input
    ANSELAbits.ANSA0 = 0;       // RA0 = analogique 
    ADCON1 = 0x80; // Référence VDD/VSS 
    ADCON0 = 0x01; // Allumer ADC, et sélectionner AN0
    TRISD &= 0xFE; // seule la première led peut être allumée
    
    RD0PPS = 0b01111;  // On branche D0 sur la valeur de PWM4
    PWM4CONbits.PWM4EN = 1; // Activation de PWM4
    setupTimer2();
   
    
    while (1) {
        unsigned int adc_value = ADC_Read();
        __delay_ms(100);
      
        long int adc_value_percent = (long int)adc_value * 100 >> 10; // Décalage à droite = division
        set_PWM4_duty_cycle(adc_value_percent);
    }
}

