#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC
#include "utils.h"      // Définition des fonctions utiles (setup)

#define _XTAL_FREQ 8000000


unsigned int ADC_Read() {
    ADCON0bits.GO = 1; // Commencer la conversion
    while (ADCON0bits.GO); // Attendre la fin de la conversion 
    return ((ADRESH << 8) + ADRESL); // Retourner le résultat (10 bits)
}

void main(void) {
    /* Code d'initialisation */
    // ON CONFIGURE LES BITS 
    // Entrée potentiomètre = entrée analogue sur RA0
    TRISAbits.TRISA0 = 1;        // RA0 = input
    ANSELAbits.ANSA0 = 0;       // RA0 = analogique 
    ADCON1 = 0x80; // Référence VDD/VSS 
    ADCON0 = 0x01; // Allumer ADC, et sélectionner AN0
    TRISD &= 0xF0; // Configurer les LEDs
    TRISB &= 0xF0;
    
    
    while (1) {
        unsigned int adc_value = ADC_Read();
        __delay_ms(100);
        
        if (adc_value < 128) {
            // Allumer RD0
            LATD = 0x01;
            LATB = 0x00;
            
        }  else if (adc_value < 256) {
            // Allumer RD1
            LATD = 0b00000010;
            LATB = 0x00;
            
        }
         else if (adc_value < 384) {
            // Allumer RD2
            LATD = 0b00000100;
            LATB = 0x00;
        }
         else if (adc_value < 512) {
            // Allumer RD3
            LATD = 0b00001000;
            LATB = 0x00;
        }
         else if (adc_value < 640) {
            // Allumer RB0
            LATD = 0x00;
            LATB = 0x01;
        }
         else if (adc_value < 768) {
             // Allumer RB1
            LATD = 0x00;
            LATB = 0b00000010;
        }
         else if (adc_value < 896) {
             // Allumer RB2
            LATD = 0x00;
            LATB = 0b00000100;
        }
         else if (adc_value < 1024) {  // Valeur max : 1023 
             // Allumer RB3
            LATD = 0x00;
            LATB = 0b00001000;
        }
        
        
        
    }
}

