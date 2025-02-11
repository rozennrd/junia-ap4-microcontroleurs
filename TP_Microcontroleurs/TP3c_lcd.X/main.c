/*
 * File:   main.c
 * Author: rozenn
 *
 * Created on 11 February 2025, 10:11
 */


#include <xc.h>
#include "lcd.h"
#include "spi.h"


void main(void) {
    SPI_InitializePins();
    LCD_InitializePins();
    SPI_Initialize();
    LCD_Initialize();
    LCD_Clear();
    LCD_GoTo(0,0);
    LCD_WriteString("Hello World!");
    
    while(1);
}
