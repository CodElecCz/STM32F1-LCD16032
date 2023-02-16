/*
 * ST7920.h
 *
 *  Created on: 15.02.2023
 *      Author: radomir.turca@codelec.cz
 */

#ifndef ST7920_H_
#define ST7920_H_

#include <stdint.h>
#include "u8g2.h"

void LCD_Init(void);
void LCD_WriteCmd(uint8_t cmd);
void LCD_WriteData(uint8_t data);
void LCD_PutStr(uint8_t x, uint8_t y, const uint8_t *ASC_GB);

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);

#endif /* ST7920_H_ */
