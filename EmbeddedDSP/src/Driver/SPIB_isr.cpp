#include "SPIB_isr.h"

#include <platform_include.h>		/* System and IOP register bit and address definitions. */
#include <stdio.h>    				/* Get declaration of printf. */
#include <sru.h>					/* Get declaration of SRU.*/
#include <stdint.h>    				/* Get definition of uint32_t. */
#include "ADDS_21479_EzKit.h"
#include "../blockProcess_audio.h"
#include "flash_edit.h"

char upgrade_buffer[UPGRADE_BUFFER_SIZE] = {0};
unsigned int upgrade_size = 0;
unsigned int upgrade_index = 0;
unsigned int upgrade_flag = 0;
unsigned int upgrade_rate = 0;
unsigned int upgrade_result = 0;

void initSPIB_SRU(void)
{
	/* SRU() for DAI, SRU2() for DPI */
	SRU2(DPI_PB07_O, SPIB_MOSI_I);
	SRU2(SPIB_MISO_O, DPI_PB06_I);
	SRU2(DPI_PB08_O, SPIB_CLK_I);
	SRU2(DPI_PB09_O, SPIB_DS_I);

	SRU2(LOW, DPI_PBEN07_I);
	SRU2(HIGH, DPI_PBEN06_I);
	SRU2(LOW, DPI_PBEN08_I);
	SRU2(LOW, DPI_PBEN09_I);
}

void setupSPIB(void)
{
	/* First configure the SPIB control registers */
	/* First clear a few registers */
	*pSPICTLB = TXFLSH|RXFLSH;
	*pSPICTLB = 0;
	*pSPIFLGB = 0;

	/* Now setup the SPI control register to : */
	*pSPICTLB = WL32 | TIMOD1 | SPIEN | MSBF;
}

void SPIBISR(uint32_t iid, void *handlerarg)
{
	int command;
    int head;
    int tail;

	/* wait for the SPI port to indicate that it has finished */
	while(1)
	{
		if (SPIF & *pSPISTATB)
		{
			if(DEBUG_SPI)
				printf("start to receive data\n");
			break;
		}
		else
		{
			if(DEBUG_SPI)
				printf("wait for the data...\n");
			asm("nop;");
		}
	}

	/* copy the SPI data into the raw data buffer */
    command = *pRXSPIB;

	/* decode command into header and tail */
    head = (command >> 24 & 0xFF);
    tail = (command & 0xFFFFFF);

	switch (head)
	{
//		case 0x70:
//			pickup.gain = (float)(tail & 0xFFFFFF) / 65535.0f;
//			*pTXSPIB = command;
//			break;
//		case 0x71:
//			*pTXSPIB = (pickup.gain * 65535.0f) + (0x71<<24);
//			break;
//		case 0x72:
//			*pTXSPIB = (pickup.peak * 65535.0f) + (0x72<<24);
//			break;
//		case 0x73:
//			pickup.peak = 0;
//			*pTXSPIB = command;
//			break;
//		case 0x74:
//			microphone.gain = (float)(tail & 0xFFFFFF) / 65535.0f;
//			*pTXSPIB = command;
//			break;
//		case 0x75:
//			*pTXSPIB = (microphone.gain * 65535.0f) + (0x75<<24);
//			break;
//		case 0x76:
//			*pTXSPIB = (microphone.peak * 65535.0f) + (0x76<<24);
//			break;
//		case 0x77:
//			microphone.peak = 0;
//			*pTXSPIB = command;
//			break;
		case 0x78:
			break;
		case 0x79:
			break;
		case 0x7A:
			break;
//		case 0x7B:
//			tone.pulseWidth = (tail & 0xFFFF) / 65535.0f;
//			*pTXSPIB = command;
//			break;
//		case 0x7C:
//			bus_mute = (float)(tail & 0xFFFFFF) / 65535.0f;
//			*pTXSPIB = command;
//			break;
		case 0x80:
			*pTXSPIB = reset_upgrade_flags(command, head, tail);
			break;
		case 0x81:
			*pTXSPIB = declare_size(command, head, tail);
			break;
		case 0x82:
			*pTXSPIB = check_upgrade_result(command, head, tail);
			break;
		case 0x83:
			software_reset();
			break;
		case 0x84:
			*pTXSPIB = check_update_rate(command, head, tail);
			break;
		case 0xAA:
			write_to_buffer(command, head, tail);
			*pTXSPIB = command;
			break;
		case 0xBB:
			if(assert_declare_n_receive(command, head, tail))
			{
				*pTXSPIB = (head << 24) + SUCCESS;
			}
			else
			{
				*pTXSPIB = (head << 24) + FAILURE;
			}
			break;
		case 0xCC:
			if(start_to_upgrade(command, head, tail))
			{
				*pTXSPIB = (head << 24) + SUCCESS;
				upgrade_flag = 1;
			}
			else
			{
				*pTXSPIB = (head << 24) + FAILURE;
				upgrade_flag = 0;
			}
			break;
		case 0xEE:
			*pTXSPIB = check_version(command, head, tail);
			break;
		case 0xFF:
			*pTXSPIB = (head << 24) + SUCCESS;
			break;
		default:
			*pTXSPIB = command;
			break;
	}
}

unsigned int check_version(int command, int head, int tail)
{
	return (head << 24) + DSP_GENERAL_VERSION;
}

unsigned int declare_size(int command, int head, int tail)
{
	/* declare buffer size */
	upgrade_size = tail*3;
	upgrade_index = 0;
	return (head << 24) + SUCCESS;
}

void write_to_buffer(int command, int head, int tail)
{
	/* write firmware file to buffer */
    upgrade_buffer[upgrade_index*3] = (command & 0x0000FF);
    upgrade_buffer[upgrade_index*3+1] = (command & 0x00FF00)>>8;
    upgrade_buffer[upgrade_index*3+2] = (command & 0xFF0000)>>16;
	upgrade_index = upgrade_index + 1;
}

unsigned int check_upgrade_result(int command, int head, int tail)
{
	if(upgrade_result)
	{
		return (head << 24) + SUCCESS;
	}
	else
	{
		return (head << 24) + FAILURE;
	}
}

unsigned int assert_declare_n_receive(int command, int head, int tail)
{
	if(upgrade_index*3 == upgrade_size)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned int start_to_upgrade(int command, int head, int tail)
{
	/* ensure buffer size and index is same */
	if(DEBUG_SPI)
	{
		printf("index: %d\n", upgrade_index*3);
		printf("index: %d\n", upgrade_size);
	}
	return assert_declare_n_receive(command, head, tail);
}

unsigned int reset_upgrade_flags(int command, int head, int tail)
{
	upgrade_size = 0;
	upgrade_index = 0;
	upgrade_flag = 0;
	upgrade_rate = 0;
	upgrade_result = 0;

	return (head << 24) + SUCCESS;
}

void software_reset()
{
	/* reset SPI registers. */
    *pSPICTL=0x0;
    *pSPIFLG=0x0;
    *pSPIDMAC=0x0;
    *pSPIBAUD=0x0;

    /* PMCTL register is not reset during a software reset, make sure the SPI is on so we can boot. */
    *pPMCTL1 &= ~SPIOFF;

    /* put PLL in bypass mode. */
    *pPMCTL |= PLLBP; /* should NOT be done in the same clock cycle as setting the multiplier. */

    /* wait for around 4096 cycles for the pll to lock. */
    int i = 0;
    for (i = 0; i < 4096; i++)
    {
    	asm("nop;");
    }

    *pSYSCTL = SRST; // software reset
}

unsigned int check_update_rate(int command, int head, int tail)
{
	return (head << 24) + upgrade_rate;
}
