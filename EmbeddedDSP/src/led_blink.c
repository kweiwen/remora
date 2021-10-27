#include "led_blink.h"
#include <sru.h>

void Clear_LED(void)
{
	SRU(HIGH, DAI_PB18_I);
}

void Set_LED(void)
{
	SRU(LOW, DAI_PB18_I);
}

void Blink_LED(int count, int duration)
{
	int flag = (count%duration*2)/duration;
	if(flag)
	{
		Clear_LED();
	}
	else
	{
		Set_LED();
	}
}
