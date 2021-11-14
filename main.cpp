#include <stdint.h>
#include <string>
#include <vector>
#include "nvic.h"
#include "tim2345.h"
#include "delay.h"
#include "rcc.h"
#include "gpio.h"
#include "adc.h"
#include "st7735.h"

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

int main(void)
{
	clock_72Mhz();

	st7735_init(4);
	st7735_fill(BLACK);

	GPIOA_clock_enable();
	GPIOA_mode(INPUT_ANALOG, 0);

	RCC_CFGR |= 1<<15;	//ADC1 clock=12MHZ
	
	ADC1_clock_enable();
	ADC1_CR2 |= 1<<1;
	ADC1_CR2 |= 1<<0;
	ms(1);
	ADC1_CR2 |= 1<<0;

	std::vector<uint8_t> buffer (160, 64);

	while (1)
	{
		buffer.emplace_back(127 - (ADC1_DR >> 5));
		buffer.erase(buffer.begin());
		for(u8 i = 0; i < 159; i++)
		{
			st7735_line(i, buffer[i], i+1, buffer[i+1], YELLOW);
		}
		ms(10);
		for(u8 i = 0; i < 159; i++)
		{
			st7735_line(i, buffer[i], i+1, buffer[i+1], BLACK);
		}
	}
}