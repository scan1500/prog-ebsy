#include <LPC177x_8x.h>
#include <stdint.h>

void yield(void);
void ledrow_1(void);
void ledrow_2(void);
void firstContext(uintptr_t first_stack);
void switchContext(uintptr_t old_stack, uintptr_t new_stack);

// Stack-Frames &stack = 0x1000 1000:
uint32_t stack[3][32];

int main(void)
{  
		static uint32_t tick = 0;
		LPC_GPIO0->DIR = 0xFF00;
		LPC_GPIO0->SET = 0x100;
	
		//Methode zum erstmaligen Starten eines Prozesses firstContext:
		//Darin „Fake“ Stack-Frame aufbauen
		//Prozesstabelle initialisieren (createProcess...)
		//Ersten Prozess auswählen, dann firstContext() aufrufen
}

void ledrow_1(void)
{
		static uint32_t led = 0x100;
		static char dir_ledrow_1 = 0;
		LPC_GPIO0->CLR = led;

		if(dir_ledrow_1)
		{
				LPC_GPIO0->SET = led>>=1;
				if(led == 0x100)
						dir_ledrow_1 = 0;
		}
		else
		{
				LPC_GPIO0->SET = led<<=1;
				if(led == 0x800)
						dir_ledrow_1 = 1;
		}
		yield();
}

void ledrow_2(void)
{
		static uint32_t led = 0x1000;
		static char dir_ledrow_2 = 0;
		LPC_GPIO0->CLR = led;

		if(dir_ledrow_2)
		{
				LPC_GPIO0->SET = led>>=1;
				if(led == 0x1000)
						dir_ledrow_2 = 0;
		}
		else
		{
				LPC_GPIO0->SET = led<<=1;
				if(led == 0x8000)
						dir_ledrow_2 = 1;
		}
		yield();
}

void yield()
{
	/*
			1. Rücksprungadresse (LR) sichern
			2. Kontext von Prozess 1 sichern R4-R12
			3. Nächsten Prozess auswählen: Prozess 2
			4. Kontext von Prozess 2 wiederherstellen
			5. Prozess 2 fortsetzen
	*/
	
		if(tick % 100000 == 0)
				switchContext();
		if(tick % 200000 == 0) 
				switchContext();
		
		tick++;
}