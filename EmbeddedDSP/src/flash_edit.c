#include "flash_edit.h"

#include "def21479.h"
#include "Cdef21479.h"
#include "sru21479.h"
#include "ADDS_21479_EzKit.h"
#include "serial_flash.h"
#include "led_blink.h"

unsigned int firmware_upgrade (char* ldr_file, int size_of_ldr)
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

unsigned int coefficients_write (char* coefficients, int size_of_coefficients)
{
    int i = 0;
    int temp;
    int offset = 0x80000;
    while(i < size_of_coefficients)
    {
    	if((i%0x10000)==0)
    	{
    		Sector_Erase(i+offset);
    	}

    	Write_Flash(i+offset, coefficients[i]);
    	temp = Read_Flash(i+offset);

    	if(temp!=coefficients[i])
    	{
    		puts("Error");
    		/* make sure LED is enable after update failed */
    		Set_LED();
    		return 0;
    	}

    	i++;
    }

    /* make sure LED is disable after firmware updated */
    Clear_LED();
	return 1;
}

void coefficients_read (char* coefficients, int size_of_coefficients)
{
    int i = 0;
    int offset = 0x80000;
    while(i < size_of_coefficients)
    {
    	coefficients[i] = Read_Flash(i+offset);
    	i++;
    }
}

void float2char(float input_data, char* output_data)
{
	/* this assign works on ADSP platform only due to the CHAR_BIT = 32 architecture */
	char* temp;
	temp = (char*)&input_data;

	output_data[0] = (*temp>>24)&0xFF;
	output_data[1] = (*temp>>16)&0xFF;
	output_data[2] = (*temp>>8 )&0xFF;
	output_data[3] = (*temp>>0 )&0xFF;
}

float char2float(char* input_data)
{
	int hexvalue = (input_data[0]<<24|input_data[1]<<16|input_data[2]<<8|input_data[3]<<0);
	float output_data = *((float*)&hexvalue);
	return output_data;
}
