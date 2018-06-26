#include "tick.h"
#include "sys.h"
#include "misc.h"

uint32_t TICK_Count;
/* SystemFrequency / 1000			1ms
 * SystemFrequency / 100000		10us
 * SystemFrequency / 1000000	1us
 */
void TICK_Config(void)
{
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		while (1); 
	}
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
 
void TICK_DelayMs(uint32_t nTime)
{ 
    TICK_Count = nTime;
    SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
    while(TICK_Count != 0);
}

void TICK_DelaySecond(uint32_t nTime)
{ 
    TICK_Count = nTime*1000;
    SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
    while(TICK_Count != 0);
}

void SysTick_Handler(void)
{
    if (TICK_Count != 0x00)
    { 
        TICK_Count--;
    }
		else
		{
			  SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
		}
}
