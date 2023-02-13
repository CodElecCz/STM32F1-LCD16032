/*
 * ST7920.c
 *
 *  Created on: 3 Mart 2019
 *      Author: fatih
 */

#include "ST7920.h"
#include "main.h"
#include "delay.h"

/*
 *  GLCD_PIN	SPI
 *  E   ------> CLK
 *  RS  ------> CS
 *  RW  ------> Data (SID)
 */



void st7920_io_cfg(void)
{
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
}

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	uint8_t res = 1;

	switch(msg)
	{
    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
    	st7920_io_cfg();
    	break;

    case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
    	HAL_Delay_us(arg_int);
    	break;

    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
    	HAL_Delay_us(arg_int);
    	break;

    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
    	HAL_Delay_us(arg_int * 10);
    	break;

    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
    	HAL_Delay_ms(arg_int);
    	break;

    case U8X8_MSG_GPIO_E:
    	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
    	break;
    case U8X8_MSG_GPIO_DC:
		HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, arg_int? GPIO_PIN_SET:GPIO_PIN_RESET);
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
