#ifndef TURBOLCD_H_
#define TURBOLCD_H_

#include "stm32F1xx_hal.h"


void LCD_Init(void);
void LCD_Clear(void);
void LCD_Set_Cursor(unsigned char row, unsigned char col);
void LCD_Write_Char(char);
void LCD_Write_String(char*);

void LCD_CMD(char cmd);
void LCD_DATA(char data);

// mode = 0 -> cmd, mode = 1 -> character
void LCD_Write_I2C(char mode, char data, char backligth);

#endif /* LCD_I2C_H_ */
