
/* Explorer 8 LCD Source File - APIs for the Explorer 8 LCD

 * File originally generated (MCC 3.45) by Microchip for the Explorer 8 (Device : PIC16F1719)
 * File amended (01/2023) by AP for JUNIA ISEN AP4

 */

#include <xc.h>
#include "lcd.h"
#include "spi.h"

#define _XTAL_FREQ 8000000
#define LCD_CHIP_SELECT      LATAbits.LATA2
#define LCD_CHIP_SELECT_TRIS TRISAbits.TRISA2

/**
  Section: Macro Declarations
 */
#define LCD_CLEAR               0x01
#define LCD_VDD_EN              0x20
#define LCD_FUNCTION_SET        0x3C
#define LCD_SET_DISPLAY         0x0C
#define LCD_SET_DDRAM_ADDRESS   0x80
#define LINE1_START_ADDRESS     0x80
#define LINE2_START_ADDRESS     0xC0
#define OUTPUT_DIR              0x00

/**
  Section: Explorer 8 LCD APIs
 */
void LCD_InitializePins(void){
    LCD_CHIP_SELECT      = 1; // Init high (active low)
    LCD_CHIP_SELECT_TRIS = 0; // Output
}

void LCD_Initialize(void) {
    LCD_WriteIOExpander(IO_DIR_A_ADDRESS, OUTPUT_DIR);
    LCD_WriteIOExpander(IO_DIR_B_ADDRESS, OUTPUT_DIR);
    LCD_WriteIOExpander(GPIO_A_ADDRESS, LCD_VDD_EN);
    __delay_ms(10);
    LCD_WriteCommand(LCD_FUNCTION_SET);
    __delay_ms(10);
    LCD_WriteCommand(LCD_SET_DISPLAY);
    __delay_ms(10);
    LCD_WriteCommand(LCD_CLEAR);
    __delay_ms(10);
    LCD_WriteCommand(LCD_SET_DISPLAY);
    __delay_ms(130);
    LCD_WriteCommand(LCD_SET_DDRAM_ADDRESS);
    __delay_ms(1);
}

void LCD_WriteIOExpander(char reg, char data) {
    LCD_CHIP_SELECT = 0;
    SPI_Exchange8bit(IO_EXPD_ADDRESS);
    SPI_Exchange8bit(reg);
    SPI_Exchange8bit(data);
    LCD_CHIP_SELECT = 1;
}

void LCD_WriteCommand(char cmd) {
    LCD_WriteIOExpander(GPIO_A_ADDRESS, 0x60);  //RS LOW -- E HIGH -- LCD Enabled 
    LCD_WriteIOExpander(GPIO_B_ADDRESS, cmd);   //Write the command on PORT B
    LCD_WriteIOExpander(GPIO_A_ADDRESS, 0x20);  //RS LOW -- E LOW -- LCD Enabled
}

void LCD_WriteByte(char data) {
    LCD_WriteIOExpander(GPIO_A_ADDRESS, 0xE0);  //RS HIGH -- E HIGH -- LCD Enabled --> This is to choose the data register on the LCD
    LCD_WriteIOExpander(GPIO_B_ADDRESS, data);  //Write the byte on PORT B
    LCD_WriteIOExpander(GPIO_A_ADDRESS, 0xA0);  //RS HIGH -- E LOW -- LCD enabled --> This is to latch the data on the LCD
}

void LCD_WriteString(const char *data) {
    char i = 0;
    while (data[i]) {
        LCD_WriteByte(data[i++]);
    }
}

void LCD_GoTo(char row, char column) {
    if (row < 2) {
        char pos = (row == 0) ? (LINE1_START_ADDRESS | column) : (LINE2_START_ADDRESS | column);
        LCD_WriteCommand(pos);
    }
}

void LCD_Clear(void) {
    LCD_WriteCommand(LCD_CLEAR);
    LCD_GoTo(0, 0);
}

