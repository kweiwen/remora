#include "def21479.h"
#include "Cdef21479.h"
#include "sru21479.h"
#include "ADDS_21479_EzKit.h"
#include "firmware_upgrade.h"
#include "serial_flash.h"
#include "led_blink.h"

unsigned int firmware_upgrade (unsigned int* ldr_file, int size_of_ldr)
{
    int i = 0;
    int temp;
    int offset = 0x10000;
    while(i < size_of_ldr)
    {
    	if((i%0x10000)==0)
    	{
    		Sector_Erase(i+offset);
    	}

    	Write_Flash(i+offset, ldr_file[i]);
    	temp = Read_Flash(i+offset);

    	if(temp!=ldr_file[i])
    	{
    		puts("Error");
    		/* make sure LED is enable after update failed */
    		Set_LED();
    		return 0;
    	}

    	i++;
    	upgrade_rate = i;
    	Blink_LED(i, 8192);
    }

    /* make sure LED is disable after firmware updated */
    Clear_LED();
	return 1;
}
