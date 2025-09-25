#include "drivers/TurboLCD.h"

extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly
#define SLAVE_ADDRESS_LCD 0x27 // change this according to ur setup

#define I2C_TX_TIMEOUT 100

#define CMD_MODE 0
#define CHAR_MODE 1

static char BACKLIGHT = 1;

void LCD_Init(void)
{
		// 4 bit initialisation
		HAL_Delay(50);  // PowerOn delay >15ms

		// 3 Times init in 8 bit mode (datasheet requirement)
		LCD_CMD (0x03);
		HAL_Delay(10);  // wait for >4.1ms
		LCD_CMD (0x03);
		HAL_Delay(1);  // wait for >100us
		LCD_CMD (0x03);
		HAL_Delay(10);

		// 4 bit mode
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
		//LCD_CMD (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
		//HAL_Delay(1);
		//LCD_CMD (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
		//HAL_Delay(1);
		LCD_CMD (0x01);  // clear display
		HAL_Delay(3);
}

void LCD_Clear (void)
{
	LCD_CMD (0x01);
	HAL_Delay(3);
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

void LCD_Backlight(char state)
{
	if (state == 1)
		BACKLIGHT = 1;
	else
		BACKLIGHT = 0;

	LCD_CMD(0x00);
}

void LCD_Write_String(char* str)
{
	while (*str) LCD_DATA(*str++);
}

void LCD_CMD(char cmd)
{
	// If I2C
	LCD_Write_I2C(CMD_MODE, cmd, BACKLIGHT);
}

void LCD_DATA(char data)
{
	// IF I2C
	LCD_Write_I2C(CHAR_MODE, data, BACKLIGHT);
}


/* I2C driver to LCD connection
 *
 * P0 = RS
 * P1 = RW
 * P2 = EN
 * P3 = BL
 * P4 = D4
 * P5 = D5
 * P6 = D6
 * P7 = D7
 *
 */
void LCD_Write_I2C(char mode, char data, char backligth)
{
	uint16_t buff_size = 4;
	uint8_t buff[buff_size];
if (backligth == 1)
{
	if(mode == 0){
		buff[0] = (data & 0xf0)|0x0C;  //BL=1, EN=1, RW = 0, RS=0
		buff[1] = (data & 0xf0)|0x08;  //BL=1, EN=0, RW = 0, RS=0
		// writing on EN falling edge
		buff[2] = ((data & 0x0f)<<4) |0x0C;  //BL=1, EN=1, RW = 0, RS=0
		buff[3] = ((data & 0x0f)<<4) |0x08;  //BL=1, EN=1, RW = 0, RS=0
	}
	else if(mode == 1){
		buff[0] = (data & 0xf0)|0x0D;  //BL=1, EN=1, RW = 0, RS=1
		buff[1] = (data & 0xf0)|0x09;  //BL=1, EN=0, RW = 0, RS=1
		// writing on EN falling edge
		buff[2] = ((data & 0x0f)<<4) |0x0D;  //BL=1, EN=1, RW = 0, RS=1
		buff[3] = ((data & 0x0f)<<4) |0x09;  //BL=1, EN=1, RW = 0, RS=1
	}
	else
		return;
}
else
{
	if(mode == 0){
		buff[0] = (data & 0xf0)|0x04;  //BL=1, EN=1, RW = 0, RS=0
		buff[1] = (data & 0xf0)|0x00;  //BL=1, EN=0, RW = 0, RS=0
		// writing on EN falling edge
		buff[2] = ((data & 0x0f)<<4) |0x04;  //BL=1, EN=1, RW = 0, RS=0
		buff[3] = ((data & 0x0f)<<4) |0x00;  //BL=1, EN=1, RW = 0, RS=0
	}
	else if(mode == 1){
		buff[0] = (data & 0xf0)|0x05;  //BL=1, EN=1, RW = 0, RS=1
		buff[1] = (data & 0xf0)|0x01;  //BL=1, EN=0, RW = 0, RS=1
		// writing on EN falling edge
		buff[2] = ((data & 0x0f)<<4) |0x05;  //BL=1, EN=1, RW = 0, RS=1
		buff[3] = ((data & 0x0f)<<4) |0x01;  //BL=1, EN=1, RW = 0, RS=1
	}
	else
		return;
}
	// Address is 7 bits and HAL uses 8 bit format with lsb free for write/read operation
	HAL_I2C_Master_Transmit(&hi2c1, (SLAVE_ADDRESS_LCD<<1), buff, buff_size, I2C_TX_TIMEOUT);
}
