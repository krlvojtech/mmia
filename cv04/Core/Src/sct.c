/*
 * sct.c
 *
 *  Created on: 19. 2. 2020
 *      Author: student
 */

#include "stm32f0xx.h"
#include "sct.h"
#include "main.h"

static const uint32_t reg_values[4][10] = {
 {
 //PCDE--------GFAB @ DIS1
 0b0111000000000111 << 16,
 0b0100000000000001 << 16,
 0b0011000000001011 << 16,
 0b0110000000001011 << 16,
 0b0100000000001101 << 16,
 0b0110000000001110 << 16,
 0b0111000000001110 << 16,
 0b0100000000000011 << 16,
 0b0111000000001111 << 16,
 0b0110000000001111 << 16,
 },
 {
 //----PCDEGFAB---- @ DIS2
 0b0000011101110000 << 0,
 0b0000010000010000 << 0,
 0b0000001110110000 << 0,
 0b0000011010110000 << 0,
 0b0000010011010000 << 0,
 0b0000011011100000 << 0,
 0b0000011111100000 << 0,
 0b0000010000110000 << 0,
 0b0000011111110000 << 0,
 0b0000011011110000 << 0,
 },
 {
 //PCDE--------GFAB @ DIS3
 0b0111000000000111 << 0,
 0b0100000000000001 << 0,
 0b0011000000001011 << 0,
 0b0110000000001011 << 0,
 0b0100000000001101 << 0,
 0b0110000000001110 << 0,
 0b0111000000001110 << 0,
 0b0100000000000011 << 0,
 0b0111000000001111 << 0,
 0b0110000000001111 << 0,
 },
 {
 //@ LEDS
 0b0000000000000000 << 16,
 0b0000000100000000 << 16,
 0b0000001100000000 << 16,
 0b0000011100000000 << 16,
 0b0000111100000000 << 16,
 0b0000111110000000 << 16,
 0b0000111111000000 << 16,
 0b0000111111100000 << 16,
 0b0000111111110000 << 16,
 0b0000111111110000 << 16,
 },
 };


void sct_init(void)
{
	sct_led(0);
	HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_NOE_Pin, 0);
}

void sct_led(uint32_t value1)
{
	for(uint32_t i=0;i<=31;i++)
	{
		if(value1 & 0x0000001)
		{
			HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_SDI_Pin, 1);
		}
		else
		{
			HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_SDI_Pin, 0);
		}
		value1 >>= 1;
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 1);
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 0);
	}
	HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_NLA_Pin, 1);
	HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_NLA_Pin, 0);
}

void sct_value(uint32_t value2, uint32_t led)
{
	uint32_t reg =0;
	reg |= reg_values[0][value2 / 100 % 10];
	reg |= reg_values[1][value2 / 10 % 10];
	reg |= reg_values[2][value2 % 10];
	reg |= reg_values[3][led];
	sct_led(reg);
}


