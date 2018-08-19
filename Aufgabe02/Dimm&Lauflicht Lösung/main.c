#include <LPC177x_8x.h>
#include <stdint.h>

void led_switch(uint32_t led_value, uint8_t led_on)
{
		if(led_on)
				LPC_GPIO0->SET = led_value;	
		else
				LPC_GPIO0->CLR = led_value;
}

int main(void)
{  
		uint32_t tick = 0;
		uint8_t direction = 0; /* direction = 0 -> left shift | direction = 1 -> right shift */
		uint32_t led_values[8] = {0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000};
		uint32_t led_full = led_values[0];

		LPC_GPIO0->DIR = 0x0000FF00;
		LPC_GPIO0->SET = led_full;
	
		while(1)
		{
				for(int led_index = 0; led_index < 8; led_index++)
				{
						if(led_values[led_index] == led_full)
							continue;
						if(tick % 8 == 0)
							led_switch(led_values[led_index], 1);
						else 
							led_switch(led_values[led_index], 0);
				}
				if(tick % 10000 == 0)
				{
						if(direction)
						{
							 led_full>>=1;
							 if(led_full == 0x100)
							 direction = 0;
						}
						else
						{
							 led_full<<=1;
							 if(led_full == 0x8000)
							 direction = 1;
						}
				}
				tick++;
		}
}
