/*
 * ST7920.c
 *
 *  Created on: 15.02.2023
 *      Author: radomir.turca@codelec.cz
 */

#include "ST7920.h"
#include "main.h"
#include "delay.h"

#define DELAY_LCD_EN_US		10	//8
#define DELAY_LCD_RS_US		50	//40

/*
 	 RS(CS*) 	Register select/(Chip select) for serial mode
 	 RW(SID*) 	Read write control / (serial data input)
 	 E(SCLK*)	Enable trigger/(serial clock)
 	 D4 to D7	Higher nibble data bus for 8 bit interface
 	 D0 to D3   Lower nibble data bus for 8 bit interface

 	 ST7920 is in parallel mode by pulling up PSB pin
 	 ST7920 is in serial interface mode when pull down PSB pin
 */

/*
 * DISPLAY STATUS
		D : Display ON/OFF control bit
			When D = "1", display ON
			When D = "0",display OFF , the content of DDRAM is not changed
		C : Cursor ON/OFF control bit
			When C = "1", cursor ON.
			When C = "0", cursor OFF.
		B : Blink ON/OFF control bit
			When B = "1", cursor position blink ON. Then display data in cursor position will blink.
			When B = "0", cursor position blink OFF

 * ENTRY MODE SET
 		I/D :address counter increase / decrease
			When I/D = "1", cursor moves right, DRAM address counter（AC）add by 1.
			When I/D = "0", cursor moves left, DRAM address counter（AC）subtract by 1.
		S: Display shift

 */

static GPIO_TypeDef * SID_PORT[8] =
{
		LCD_DB7_GPIO_Port,
		LCD_DB6_GPIO_Port,
		LCD_DB5_GPIO_Port,
		LCD_DB4_GPIO_Port,
		LCD_DB3_GPIO_Port,
		LCD_DB2_GPIO_Port,
		LCD_DB1_GPIO_Port,
		LCD_DB0_GPIO_Port
};

static uint16_t SID_PIN[8] =
{
		LCD_DB7_Pin,
		LCD_DB6_Pin,
		LCD_DB5_Pin,
		LCD_DB4_Pin,
		LCD_DB3_Pin,
		LCD_DB2_Pin,
		LCD_DB1_Pin,
		LCD_DB0_Pin
};

void LCD_WriteCmd(uint8_t cmd)
{

	GPIO_PinState rs = HAL_GPIO_ReadPin(LCD_RS_GPIO_Port, LCD_RS_Pin);
	if(rs)
	{
		HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
		HAL_Delay_us(DELAY_LCD_RS_US);
	}

	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);

	for(int i = 0; i < 8; i++)
	{
		if((cmd << i) & 0x80)
			HAL_GPIO_WritePin(SID_PORT[i], SID_PIN[i], GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(SID_PORT[i], SID_PIN[i], GPIO_PIN_RESET);
	}

	HAL_Delay_us(DELAY_LCD_EN_US);
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
	HAL_Delay_us(DELAY_LCD_EN_US);
}

void LCD_WriteData(uint8_t data)
{
	GPIO_PinState rs = HAL_GPIO_ReadPin(LCD_RS_GPIO_Port, LCD_RS_Pin);
	if(!rs)
	{
		HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);
		HAL_Delay_us(DELAY_LCD_RS_US);
	}

	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);

	for(int i = 0; i < 8; i++)
	{
		if((data << i) & 0x80)
			HAL_GPIO_WritePin(SID_PORT[i], SID_PIN[i], GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(SID_PORT[i], SID_PIN[i], GPIO_PIN_RESET);
	}

	HAL_Delay_us(DELAY_LCD_EN_US);
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
	HAL_Delay_us(DELAY_LCD_EN_US);
}

void LCD_Init(void)
{
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);

	HAL_Delay_ms(2*50);
	LCD_WriteCmd(0x30);
	HAL_Delay_us(2*110);
	LCD_WriteCmd(0x30);
	HAL_Delay_us(2*40);
	LCD_WriteCmd(0x0C);
	HAL_Delay_us(2*110);
	LCD_WriteCmd(0x01);
	HAL_Delay_ms(2*11);
	LCD_WriteCmd(0x06);
}

void LCD_SetCursor(uint8_t x, uint8_t y)
{
	uint8_t CharSite;

	x = x & 0x0f;

	y = y & 0x0f;
	y = y << 4;
	y = y & 0x30;

	CharSite= x | y | 0x80;

	LCD_WriteCmd(CharSite);
}

void LCD_PutStr(uint8_t x, uint8_t y, const uint8_t *ASC_GB)
{
	LCD_SetCursor(x, y);

	while(*ASC_GB > 0 && (*ASC_GB != '\0'))
	{
		LCD_WriteData(*ASC_GB);
		ASC_GB++;
	}
}

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	uint8_t res = 1;

	switch(msg)
	{
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
    	LCD_Init();
    	break;
    case U8X8_MSG_DELAY_NANO:
    	break;
    case U8X8_MSG_DELAY_MILLI:
    	HAL_Delay_ms(arg_int);
    	break;
    case U8X8_MSG_GPIO_E:
    	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
    	HAL_Delay_us(DELAY_LCD_EN_US);
    	break;
    case U8X8_MSG_GPIO_DC:
    	{
    		GPIO_PinState rs = HAL_GPIO_ReadPin(LCD_RS_GPIO_Port, LCD_RS_Pin);
    		if(rs!=arg_int)
    		{
    			HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
    			HAL_Delay_us(DELAY_LCD_RS_US);
    		}
    	}
		break;
	case U8X8_MSG_GPIO_D0:
		HAL_GPIO_WritePin(LCD_DB0_GPIO_Port, LCD_DB0_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case U8X8_MSG_GPIO_D1:
		HAL_GPIO_WritePin(LCD_DB1_GPIO_Port, LCD_DB1_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case U8X8_MSG_GPIO_D2:
		HAL_GPIO_WritePin(LCD_DB2_GPIO_Port, LCD_DB2_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case U8X8_MSG_GPIO_D3:
		HAL_GPIO_WritePin(LCD_DB3_GPIO_Port, LCD_DB3_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case U8X8_MSG_GPIO_D4:
		HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case U8X8_MSG_GPIO_D5:
		HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case U8X8_MSG_GPIO_D6:
		HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case U8X8_MSG_GPIO_D7:
		HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	default:
		res = 0;
		return res; /* not implemented */
	}

	return res;
}
