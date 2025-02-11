#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

#define _XTAL_FREQ 8000000

volatile uint8_t clockCnt = 0;
volatile unsigned char ascend = 1;

void UART_Write(char data) {
    while(!TXIF);  // Wait for TXIF (Transmit Interrupt Flag) to be set
    TX1REG = data;  // Load data into TXREG to send
}


void UART_Write_String(const char *str) {
    while(*str) {
        
        UART_Write(*str);
        str++;
    }
}

void __interrupt() UART_ISR() {
    if (PIR1bits.RCIF) {  // Vérifie si l'interruption est déclenchée par Timer2
        
        char truc = RC1REG;
        UART_Write(truc);
        
        PIR1bits.RCIF = 0; // Efface le flag d'interruption
    }
}

char UART_Read(void) {
    while(!RCIF);  // Wait for RCIF (Receive Interrupt Flag) to be set
    return RC1REG; // Return received data from RC1REG register
}

void main(void) {
    
    INTCONbits.GIE = 1; // Enabling interrupts
    INTCONbits.PEIE = 1; // Enabling all active peripheral interrupts
    PIE1bits.RCIE = 1; // Lève le flag d'interruption quand USART reçoit
    /* Code d'initialisation */
    // ON CONFIGURE LES BITS 
    // Assignation des pins de réception et de transmission aux entrées RX/TX du module EUSART
    RC6PPS = 0b10100;
    RXPPS = 0b10111;
    
    // Configuration d'RC6 en sortie
    TRISCbits.TRISC6 = 0; // RC6 = sortie
    LATCbits.LATC6 = 0; // Initialisation des valeurs à 0;
    // Configuration d'RC7 en entrée numérique
    TRISCbits.TRISC7 = 1; // Rc7 = entrée
    ANSELCbits.ANSC7 = 0; // RC7 = numérique
    // Configuration de la vitesse de transmission à 9600 bauds à travers 3 bits de config + un registre de config
    TX1STAbits.SYNC = 0; // module en mode asynchrone 
    TX1STAbits.BRGH = 0; // page 355 question
    BAUD1CONbits.BRG16 = 1; // 16 bits mode
    SP1BRG = 51;
    TRISD = 0x00; 
    
    // Activation du module EUSART
    RC1STAbits.SPEN = 1;  // Activation des pins
    TXSTAbits.TXEN = 1; // Activation du transmitter
    RC1STAbits.CREN = 1; // Activation du reciever
    
    // Utilisation du module EUSART
    // Transmission 
    //TX1REG = 0; // Transmission de rien
    //int lecture = RC1REG; // Lecture de RC1REG.
    
    
    // NOTE : /dev/ttyACMx 
    // ACM0 = pickit
    // ACM1 = microchip explorer
    
    while (1) {
        //char data = UART_Read();
        //UART_Write(data);
        
    }
}

