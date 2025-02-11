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

void main(void) {
    /* Code d'initialisation */
    // ON CONFIGURE LES BITS 
    // Assignation des pins de r�ception et de transmission aux entr�es RX/TX du module EUSART
    RC6PPS = 0b10100;
    RXPPS = 0b10111;
    
    // Configuration d'RC6 en sortie
    TRISCbits.TRISC6 = 0; // RC6 = sortie
    LATCbits.LATC6 = 0; // Initialisation des valeurs � 0;
    // Configuration d'RC7 en entr�e num�rique
    TRISCbits.TRISC7 = 1; // Rc7 = entr�e
    ANSELCbits.ANSC7 = 1; // RC7 = num�rique
    // Configuration de la vitesse de transmission � 9600 bauds � travers 3 bits de config + un registre de config
    TX1STAbits.SYNC = 0; // module en mode asynchrone 
    TX1STAbits.BRGH = 0; // page 355 question
    BAUD1CONbits.BRG16 = 1; // 16 bits mode
    SP1BRG = 51;
    
    
    // Activation du module EUSART
    RC1STAbits.SPEN = 1;  // Activation des pins
    TXSTAbits.TXEN = 1; // Activation du transmitter
    RC1STAbits.CREN = 1; // Activation du reciever
    
    // Utilisation du module EUSART
    // Transmission 
    //TX1REG = 0; // Transmission de rien
    //int lecture = RC1REG; // Lecture de RC1REG.
     UART_Write_String("Hello world!\n\r");
    // NOTE : /dev/ttyACMx 
    // ACM0 = pickit
    // ACM1 = microchip explorer
    

     
    while (1) {
        //char data = UART_Read();
        //UART_Write(data);
        
    }
}

