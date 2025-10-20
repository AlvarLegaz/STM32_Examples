#include "drivers/TurboLCD.h"

extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly
#define SLAVE_ADDRESS_LCD 0x27 // change this according to ur setup

#define PIN_DISPLAY_RW GPIO_PIN_6 //PA6 d12
#define PIN_DISPLAY_RS GPIO_PIN_7 //PA7 d11
#define PIN_DISPLAY_EN GPIO_PIN_9 //PA9 d8

#define PIN_DISPLAY_D4 GPIO_PIN_10	//PIN DISPLAY 4 -> PB10 D6
#define PIN_DISPLAY_D5 GPIO_PIN_4	//PIN DISPLAY 5 -> PB4 D5
#define PIN_DISPLAY_D6 GPIO_PIN_5	//PIN DISPLAY 6 -> PB5 D4
#define PIN_DISPLAY_D7 GPIO_PIN_3	//PIN DISPLAY 7 -> PB3 D3

#define I2C_TX_TIMEOUT 100

#define CMD_MODE 0
#define CHAR_MODE 1

static char BACKLIGHT = 1;

void LCD_Init(void)
{
    HAL_Delay(50); // Esperar al encendido del LCD (>15ms)

    // Forzar modo 8 bits tres veces
    LCD_CMD(0x03);
    HAL_Delay(5);
    LCD_CMD(0x03);
    HAL_Delay(5);
    LCD_CMD(0x03);
    HAL_Delay(1);

    // Pasar a modo 4 bits
    LCD_CMD(0x02);
    HAL_Delay(1);

    // Configurar LCD
    LCD_CMD(0x28); // 4-bit, 2 líneas, 5x8 font
    LCD_CMD(0x08); // Display OFF, cursor OFF
    LCD_CMD(0x01); // Clear display
    HAL_Delay(2);
    LCD_CMD(0x06); // Entry mode: incrementa cursor
    LCD_CMD(0x0C); // Display ON, cursor OFF, blink OFF
    //LCD_CMD(0x0E);  // Display ON + cursor visible
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
	//LCD_Write_I2C(CMD_MODE, cmd, BACKLIGHT);

	// IF PARALLEL
	LCD_Write_PARALLEL(CMD_MODE, cmd);
}

void LCD_DATA(char data)
{
	// IF I2C
	//LCD_Write_I2C(CHAR_MODE, data, BACKLIGHT);

	// IF PARALLEL
	LCD_Write_PARALLEL(CHAR_MODE, data);
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


void LCD_Write_PARALLEL(char mode, char data)
{
    // Configura RS/RW
    HAL_GPIO_WritePin(GPIOA, PIN_DISPLAY_RS, (mode == CHAR_MODE) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PIN_DISPLAY_RW, GPIO_PIN_RESET);

    // Nibble alto
    HAL_GPIO_WritePin(GPIOB, PIN_DISPLAY_D7, (data >> 7) & 0x01);
    HAL_GPIO_WritePin(GPIOB, PIN_DISPLAY_D6, (data >> 6) & 0x01);
    HAL_GPIO_WritePin(GPIOB, PIN_DISPLAY_D5, (data >> 5) & 0x01);
    HAL_GPIO_WritePin(GPIOB, PIN_DISPLAY_D4, (data >> 4) & 0x01);
    LCD_EnablePulse();

    // Nibble bajo
    HAL_GPIO_WritePin(GPIOB, PIN_DISPLAY_D7, (data >> 3) & 0x01);
    HAL_GPIO_WritePin(GPIOB, PIN_DISPLAY_D6, (data >> 2) & 0x01);
    HAL_GPIO_WritePin(GPIOB, PIN_DISPLAY_D5, (data >> 1) & 0x01);
    HAL_GPIO_WritePin(GPIOB, PIN_DISPLAY_D4, (data >> 0) & 0x01);
    LCD_EnablePulse();
}

void LCD_EnablePulse(void)
{
    HAL_GPIO_WritePin(GPIOA, PIN_DISPLAY_EN, GPIO_PIN_SET);
    for(volatile int i = 0; i < 300; i++); // ≈ 1 µs
    HAL_GPIO_WritePin(GPIOA, PIN_DISPLAY_EN, GPIO_PIN_RESET);
    for(volatile int i = 0; i < 300; i++);
}
