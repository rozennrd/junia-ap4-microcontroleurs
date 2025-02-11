/*
 * File:   main.c
 * Author: rozenn
 *
 * Created on February 11, 2025, 10:39 AM
 */

#include <xc.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lcd.h"
#include "spi.h"
#include "configbits.h"
#define _XTAL_FREQ 8000000
#define ALPHABET_SIZE 36
const char alphabet[ALPHABET_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                  'u', 'v', 'w', 'x', 'y', 'z'};

volatile unsigned int offset = 0;
volatile int modeDechiffrement = 0;


/* POTENTIOM�TRE */
void POT_initialize() {
    TRISAbits.TRISA0 = 1;        // RA0 = input
    ANSELAbits.ANSA0 = 1;       // RA0 = analogique 
    ADCON1 = 0x80; // R�f�rence VDD/VSS 
    ADCON0 = 0x01; // Allumer ADC, et s�lectionner AN0

}

void UART_Write(char data) { // Ecrire un caract�re dans le terminal
    while(!TXIF);  
    TX1REG = data;  // �crire un caract�re dans TX1REG
}

void UART_Write_String(const char *str) { // Ecrire dans le terminal
    while(*str) {
        
        UART_Write(*str);
        str++;
    }
}

int find_index(char c) { // Trouver l'index d'un caract�re dans l'alphabet
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (alphabet[i] == c) {
            return i;
        }
    }
    return -1;  // Retourne -1 si le caract�re n'est pas trouv�
}


unsigned int getOffsetFromAdcValue(unsigned int val) {  // R�cup�rer le d�calage suivant la valeur de l'ADC
    return (val * ALPHABET_SIZE) >> 10;
}

unsigned int ADC_Read() { // R�cup�rer la valeur de l'ADC
    ADCON0bits.GO = 1; // Commencer la conversion
    while (ADCON0bits.GO); // Attendre la fin de la conversion 
    return ((ADRESH << 8) + ADRESL); // Retourner le r�sultat (10 bits)
}

char getNewChar(char toTransform) { // r�cup�rer le caract�re avec le d�calage en fonction du mode de chiffrement
     for (unsigned int i = 0; i < ALPHABET_SIZE; i++) {
        if (alphabet[i] == toTransform) {
            if (!modeDechiffrement) {
                return alphabet[(i + offset) % ALPHABET_SIZE];  // D�calage circulaire
            } else {
                 return alphabet[(i - offset) % ALPHABET_SIZE];  // D�calage circulaire
            }
            
        }
    }
    return 0;  // Si le caract�re n'est pas trouv�, on le retourne inchang�
}

void __interrupt() UART_ISR() {
    LATD= 0x0F;
    if (PIR1bits.RCIF) { // Flag d'interruption  
        
        char truc = tolower(RC1REG); // Gestion des majuscules
        // modif de truc en fonction du d�calage 
        truc = getNewChar(truc); // R�cup�ration du caract�re chiffr� / d�chiffr�
        if (!truc){ // Si le caract�re n'a pas �t� trouv�
            return; // On ne l'�crit pas
        }
        UART_Write(truc); // sinon on �crit le caract�re
        
        PIR1bits.RCIF = 0; // Efface le flag d'interruption
    }
    
}


void ECHO_initializeBits() {
    INTCONbits.GIE = 1; // Enabling interrupts
    INTCONbits.PEIE = 1; // Enabling all active peripheral interrupts
    PIE1bits.RCIE = 1; // L�ve le flag d'interruption quand USART re�oit
     
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
    ANSELCbits.ANSC7 = 0; // RC7 = num�rique
    // Configuration de la vitesse de transmission � 9600 bauds � travers 3 bits de config + un registre de config
    TX1STAbits.SYNC = 0; // module en mode asynchrone 
    TX1STAbits.BRGH = 0; // page 355 question
    BAUD1CONbits.BRG16 = 1; // 16 bits mode
    SP1BRG = 51;
    TRISD = 0x00; 
    LATD=0X00;
    
    // Activation du module EUSART
    RC1STAbits.SPEN = 1;  // Activation des pins
    TXSTAbits.TXEN = 1; // Activation du transmitter
    RC1STAbits.CREN = 1; // Activation du reciever
}

void main(void) {
    // Initialisation
    ECHO_initializeBits();
    POT_initialize();
    SPI_InitializePins();
    LCD_InitializePins();
    SPI_Initialize();
    LCD_Initialize();
    // Initialisation du bouton
    TRISAbits.TRISA5 = 1; // Le bit 5 du registre A est en entr�e
    ANSELAbits.ANSA5 = 0; // D�sactive le mode analogique
    
    
    char lcd_display[32]; // Initialisation de la variable display
    
    // Premi�re �criture 
     LCD_GoTo(1,0);
     LCD_WriteString("Chiffrement    ");
    while(1){
        // Gestion de l'ADC (d�calage)
        unsigned int valeur = ADC_Read();
        unsigned int newOffset = getOffsetFromAdcValue(valeur);
        if (offset != newOffset) {
            offset = newOffset;
            sprintf(lcd_display, "Decalage : %02d ", offset);
            LCD_GoTo(0,0);
            LCD_WriteString(lcd_display);
            
        }
        
        // Gestion du mode de chiffrement
        if(PORTAbits.RA5 == 0) {  // Lecture de l'entr�e num�rique (lorsque le bouton est press�)
            modeDechiffrement = !modeDechiffrement;
            
            if (modeDechiffrement == 0) {
                LCD_GoTo(1,0);
                LCD_WriteString("Chiffrement    ");
            }
            else {
                LCD_GoTo(1,0);
                LCD_WriteString("Dechiffrement");
            }
         }
    };
}
