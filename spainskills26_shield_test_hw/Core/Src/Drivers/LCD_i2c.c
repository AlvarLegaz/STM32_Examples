/** Put this in the src folder **/
/**No use la dirección 0x27. Esto no es arduino. STM32 usa 8 bits para la dirección (7 direcciones + 1 bit de lectura / escritura), mientras que Arduino usa 7.
Verifique la hoja de datos para verificar la dirección correcta junto con el bit de escritura / lectura. 0x27=0x4E, 0x3F=0x7E**/

//NOTA: RECORDAR ACTIVAR LOS PINES SDA Y SCL CON PULL-UP, SE PUEDE HACER EN HAL_I2C_MspInit(hi2c) O CON CUBEMX

#include "Drivers/LCD_I2C.h"

extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly
#define SLAVE_ADDRESS_LCD 0x4e // change this according to ur setup

void LCD_CMD (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void LCD_DATA (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void LCD_Clear (void)
{
	LCD_CMD (0x01);
	HAL_Delay(3);
	/*
	LCD_CMD (0x80);
	for (int i=0; i<70; i++)
	{
		LCD_DATA (' ');
	}
	*/
}

void LCD_Set_Cursor(unsigned char row, unsigned char col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
				case 2:
            col += 0x94;
            break;
				case 3:
            col += 0xD4;
            break;
    }
    LCD_CMD (col);
}


void LCD_Init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	LCD_CMD (0x03);
	HAL_Delay(5);  // wait for >4.1ms
	LCD_CMD (0x03);
	HAL_Delay(1);  // wait for >100us
	LCD_CMD (0x03);
	HAL_Delay(10);
	LCD_CMD (0x02);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	LCD_CMD (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(4);
	LCD_CMD (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	LCD_CMD (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	LCD_CMD (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
//	LCD_CMD (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
//	HAL_Delay(2);
	LCD_CMD (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	LCD_CMD (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	HAL_Delay(1);
	LCD_CMD (0x01);  // clear display
	HAL_Delay(3);

}

void LCD_Write_String (char *str)
{
	while (*str) LCD_DATA(*str++);
}

void LCD_Write_Char(char Data)
{
   char Low4,High4;
   Low4  = Data & 0x0F;
   High4 = Data & 0xF0;

   LCD_DATA(High4>>4);
   LCD_DATA(Low4);
}

void LCD_SL()
{
    LCD_CMD (LCD_DISPLAY_SHIFT_LEFT);
}
void LCD_SR()
{
    LCD_CMD (LCD_DISPLAY_SHIFT_RIGHT);
}
