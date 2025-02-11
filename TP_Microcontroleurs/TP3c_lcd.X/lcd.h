/* Explorer 8 LCD Header File - APIs for the Explorer 8 LCD

 * File originally generated (MCC 3.45) by Microchip for the Explorer 8 (Device : PIC16F1719)
 * File amended (01/2023) by AP for JUNIA ISEN AP4

 */


#ifndef LCD_H
#define	LCD_H

/**
  Section: Macro Declarations
 */
#define IO_EXPD_ADDRESS     0x40  
#define GPIO_A_ADDRESS      0x12
#define GPIO_B_ADDRESS      0x13
#define IO_DIR_A_ADDRESS    0x00
#define IO_DIR_B_ADDRESS    0x01

/**
  Section: Explorer 8 LCD Function Prototypes
 */
void LCD_InitializePins(void);
void LCD_Initialize(void);
void LCD_WriteIOExpander(char reg, char data);
void LCD_WriteCommand(char cmd);
void LCD_WriteByte(char data);
void LCD_WriteString(const char *data);
void LCD_GoTo(char row, char column);
void LCD_Clear(void);

#endif	/* LCD_H */
