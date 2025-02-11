#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC
#include "utils.h"

#define _XTAL_FREQ 8000000

volatile uint8_t clockCnt = 0;
volatile unsigned char ascend = 1;


void setupTimer2_40ms(void) {
    // Le timer 2 est setup pour 40 ms. 40 ms * 25 = 1000ms soit 1 seconde, et une division par 25 reste g�rable pour le syst�me 
    T2CONbits.T2CKPS = 0b11;  // Prescaler = 1:64
    T2CONbits.T2OUTPS = 0b0101; // Postscaler = 1:8
    PR2 = 249;                // P�riode de Timer2 pour 40ms
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



void set_PWM4_duty_cycle(long int duty_cycle_percentage) {
    uint16_t duty_cycle = (duty_cycle_percentage * (4 * (PR2 + 1))) / 100;

    PWM4DCH = (duty_cycle >> 2) & 0xFF;       // 8 bits de poids fort
    PWM4DCL = (duty_cycle & 0x03) << 6;       // 2 bits de poids faible dans les bits <7:6>
}

// Routine d'interruption
/* Notes : ISR = Interrupt service routine  - on d�finit notre routine d'interruption */
void __interrupt() ISR() {
    if (PIR1bits.TMR2IF) {  // V�rifie si l'interruption est d�clench�e par Timer2
        PIR1bits.TMR2IF = 0; // Efface le flag d'interruption
        clockCnt ++;
        
        if (ascend) { // Si on est en cycle ascendant (de 0 � 100%)
            long duty_cycle = (long)clockCnt*100/25; 
            set_PWM4_duty_cycle(duty_cycle);
        } else { // Si on est en cycle descendant (de 100 � 0 %)
            long duty_cycle = (long)(25-clockCnt)*100/25;  
            set_PWM4_duty_cycle(duty_cycle);
        }
        
        // Arriv�e au bout du cycle (1 seconde)
        
        if (clockCnt == 25) {
            ascend = !ascend; // On inverse le cycle (si ascendant : devient descendant, sinon : devient ascendant)
            clockCnt = 0; // On remet le count � 0
        }
        
    }
}



void main(void) {
    /* Code d'initialisation */
    // ON CONFIGURE LES BITS 
    // Entr�e potentiom�tre = entr�e analogue sur RA0

    TRISD &= 0xFE; // seule la premi�re led peut �tre allum�e
    
    RD0PPS = 0b01111;  // On branche D0 sur la valeur de PWM4
    PWM4CONbits.PWM4EN = 1; // Activation de PWM4
    setupTimer2_40ms();
    
   
    
    while (1) {
        // Rien, tout est dans la routine d'interruption
    }
}

