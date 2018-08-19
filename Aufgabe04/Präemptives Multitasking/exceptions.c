#include <stdint.h>
#include <LPC177x_8x.h>

void HardFault_Handler(void) 
{
		uint32_t counter = 0;
		while(1) 
		{
				if (counter % 2)
				{
						LPC_GPIO0->SET = 0x100;
						for(uint32_t i = 0; i < 240000; i++){}
				}	
				else
				{
						LPC_GPIO0->CLR = 0x100;
						for(uint32_t i = 0; i < 240000; i++){}
				}
				counter++;
		}
}

void MemManage_Handler(void) 
{
		uint32_t counter = 0;
		while(1) 
		{
				if (counter % 2)
				{
						LPC_GPIO0->SET = 0x100;
						for(uint32_t i = 0; i < 240000; i++){}
				}
				else
				{
						LPC_GPIO0->CLR = 0x100;
						for(uint32_t i = 0; i < 240000; i++){}
				}
				counter++;
		}
}
