#include <LPC177x_8x.h>
#include <stdint.h>

void led1(void)
{
		static uint32_t led = 0x100;
		static char rightshift_1 = 0;
		LPC_GPIO0->CLR = led;

		if(rightshift_1)
		{
				LPC_GPIO0->SET = led>>=1;
				if(led == 0x100)
						rightshift_1 = 0;
		}
		else
		{
				LPC_GPIO0->SET = led<<=1;
				if(led == 0x800)
						rightshift_1 = 1;
		}
}

void led2(void)
{
		static uint32_t led = 0x1000;
		static char rightshift_2 = 0;
		LPC_GPIO0->CLR = led;

		if(rightshift_2)
		{
				LPC_GPIO0->SET = led>>=1;
				if(led == 0x1000)
						rightshift_2 = 0;
		}
		else
		{
				LPC_GPIO0->SET = led<<=1;
				if(led == 0x8000)
						rightshift_2 = 1;
		}
}

int main(void)
{  
		uint32_t tick = 0;
		LPC_GPIO0->DIR = 0xFF00;
		LPC_GPIO0->SET = 0x1100;
		
		while(1)
		{
				tick++;
				if(tick % 100000 == 0)
					led1();
				if(tick % 200000 == 0) 
					led2();
		}
}

