#ifndef LCD_I2C_H_
#define LCD_I2C_H_

#include "stm32L0xx_hal.h"

#define LCD_CLEAR        0x01
#define LCD_HOME    0x02

#define LCD_DISPLAY_OFF  0x08
#define LCD_DISPLAY_ON   0x0C
#define LCD_CURSOR_ON    0x0E
#define LCD_CURSOR_BLINK 0x0F

#define LCD_CURSOR_SHIFT_RIGHT  0x14
#define LCD_CURSOR_SHIFT_LEFT   0x10
#define LCD_DISPLAY_SHIFT_RIGHT 0x18
#define LCD_DISPLAY_SHIFT_LEFT  0x1C

void LCD_Init(void);                  // Initialize The LCD For 4-Bit Interface
void LCD_Clear(void);                 // Clear The LCD Display
void LCD_SL(void);                    // Shift The Entire Display To The Left
void LCD_SR(void);                    // Shift The Entire Display To The Right
void LCD_CMD(char);      // Send Command To LCD
void LCD_DATA(char);     // Send 4-Bit Data To LCD
void LCD_Set_Cursor(unsigned char, unsigned char);  // Set Cursor Position
void LCD_Write_Char(char);        // Write Character To LCD At Current Position
void LCD_Write_String(char*);     // Write A String To LCD

#endif /* LCD_I2C_H_ */

