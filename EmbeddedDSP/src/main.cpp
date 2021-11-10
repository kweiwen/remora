/**
          _____                    _____                    _____                   _______                   _____                    _____
         /\    \                  /\    \                  /\    \                 /::\    \                 /\    \                  /\    \
        /::\    \                /::\    \                /::\____\               /::::\    \               /::\    \                /::\    \
       /::::\    \              /::::\    \              /::::|   |              /::::::\    \             /::::\    \              /::::\    \
      /::::::\    \            /::::::\    \            /:::::|   |             /::::::::\    \           /::::::\    \            /::::::\    \
     /:::/\:::\    \          /:::/\:::\    \          /::::::|   |            /:::/~~\:::\    \         /:::/\:::\    \          /:::/\:::\    \
    /:::/__\:::\    \        /:::/__\:::\    \        /:::/|::|   |           /:::/    \:::\    \       /:::/__\:::\    \        /:::/__\:::\    \
   /::::\   \:::\    \      /::::\   \:::\    \      /:::/ |::|   |          /:::/    / \:::\    \     /::::\   \:::\    \      /::::\   \:::\    \
  /::::::\   \:::\    \    /::::::\   \:::\    \    /:::/  |::|___|______   /:::/____/   \:::\____\   /::::::\   \:::\    \    /::::::\   \:::\    \
 /:::/\:::\   \:::\____\  /:::/\:::\   \:::\    \  /:::/   |::::::::\    \ |:::|    |     |:::|    | /:::/\:::\   \:::\____\  /:::/\:::\   \:::\    \
/:::/  \:::\   \:::|    |/:::/__\:::\   \:::\____\/:::/    |:::::::::\____\|:::|____|     |:::|    |/:::/  \:::\   \:::|    |/:::/  \:::\   \:::\____\
\::/   |::::\  /:::|____|\:::\   \:::\   \::/    /\::/    / ~~~~~/:::/    / \:::\    \   /:::/    / \::/   |::::\  /:::|____|\::/    \:::\  /:::/    /
 \/____|:::::\/:::/    /  \:::\   \:::\   \/____/  \/____/      /:::/    /   \:::\    \ /:::/    /   \/____|:::::\/:::/    /  \/____/ \:::\/:::/    /
       |:::::::::/    /    \:::\   \:::\    \                  /:::/    /     \:::\    /:::/    /          |:::::::::/    /            \::::::/    /
       |::|\::::/    /      \:::\   \:::\____\                /:::/    /       \:::\__/:::/    /           |::|\::::/    /              \::::/    /
       |::| \::/____/        \:::\   \::/    /               /:::/    /         \::::::::/    /            |::| \::/____/               /:::/    /
       |::|  ~|               \:::\   \/____/               /:::/    /           \::::::/    /             |::|  ~|                    /:::/    /
       |::|   |                \:::\    \                  /:::/    /             \::::/    /              |::|   |                   /:::/    /
       \::|   |                 \:::\____\                /:::/    /               \::/____/               \::|   |                  /:::/    /
        \:|   |                  \::/    /                \::/    /                 ~~                      \:|   |                  \::/    /
         \|___|                   \/____/                  \/____/                                           \|___|                   \/____/

*/

#include <services/int/adi_int.h>  			/* Interrupt Handler API header. */
#include <sru21479.h>						/* Get definition of SRU() */
#include <stdlib.h>							/* Get definition of NULL */
#include <iostream>

#include "blockProcess_audio.h"     		/* Get definition of handleCodecData() */
#include "commandProcess.h"

#include "Driver/ADDS_21479_EzKit.h" 		/* Get definition of global variable */
#include "Driver/flash_edit.h"				/* Get definition of firmware_upgrade() */
#include "Driver/heap_manager.h"
#include "Driver/init_PLL_SDRAM.h" 			/* Get definition of initPLL(), initExternalMemory() */
#include "Driver/init4272viaSPI.h"			/* Get definition of InitCS4272() */
#include "Driver/initSPORT01_I2S_mode.h"	/* Get definition of initSPORT() */
#include "Driver/initSRU.h"					/* Get definition of initDAI() */
#include "Driver/led_blink.h"				/* Get definition of Blink_LED(), Set_LED(), Clear_LED() */
#include "Driver/serial_flash.h"       		/* Get definition of initSPI() */
#include "Driver/SPIB_isr.h"     			/* Get definition of SPIBTransferISR() */
#include "Driver/SPORT1_isr.h"				/* Get definition of TalkThroughISR() */
#include "Driver/UART0_isr.h"				/* Get definition of UART0ISR(), initUART();*/
#include "Driver/heap_manager.h"

#include "AudioParameter/AudioParameter.h"
#include "AudioParameter/AudioParameterFloat.h"
#include "AudioParameter/AudioParameterInt.h"
#include "AudioParameter/AudioParameterBool.h"
#include "AudioParameter/AudioParameterChoice.h"

void initGPIO(void)
{
	/* SRU() for DAI, SRU2() for DPI */

	/* Configure UART */
    SRU2(UART0_TX_O, DPI_PB11_I); 	/* UART transmit signal is connected to DPI pin 11 */
    SRU2(HIGH, DPI_PBEN11_I);

    SRU2(DPI_PB12_O, UART0_RX_I); 	/* connect the pin buffer output signal to the UART0 receive */
    SRU2(LOW, DPI_PB12_I);
    SRU2(LOW, DPI_PBEN12_I);      	/* disables DPI pin12 as input */

    /* Configure LED */
	SRU(LOW, DAI_PB18_I); 			/* Tie Low to Pin Buffer Input */
	SRU(HIGH, PBEN18_I);			/* Tie High to Pin Buffer Enable */
}

void process_loop(void)
{
	volatile int count = 0;

	/* make sure LED is disable before processing */
	Clear_LED();

    while(1)
    {
    	if(inputReady)
    	{
    		/* audio processing data here! */
    		count++;
    		handleCodecData(buffer_cntr);
    	}
    	else if(commandReady)
    	{
    		/* command processing data here! */
    		handleCommandData();
    	}
    	else if (upgrade_flag & !isProcessing & !isCommandProcessing)
		{
			isProcessing = 0;
			inputReady = 0;
			buffer_cntr = 1;
			upgrade_flag = 0;

			/* clear buffer and chained DMAs */
			clear_currentBuffer();
			clear_currentDMA();

			/* upgrade flag will break the while loop */
			break;
		}
		/* LED status indicate the audio prcessing callback is running
		 * Blink_LED(count, 32); */
    }

    /* make sure LED is disable after processing */
    Clear_LED();
}

void upgrade_loop(void)
{
	/* init SPI Read/Write Flash */
    initSPI();

    /* start to writing data into flash memory... */
    upgrade_result = firmware_upgrade(upgrade_buffer, upgrade_size);
}

void task_loop(void)
{
	/* declare processing algorithm here */
	prepare_audioBlocks();

	/* init GPIO and SRU */
	initGPIO();

	/* init UART and install interrupt */
	initUART();
    adi_int_InstallHandler(ADI_CID_P14I, UART0ISR, NULL, true);
	adi_int_InstallHandler(ADI_CID_TMZLI, TimerISR, (void *)&timer_isr_count, true);

	/* init SPI communication and install interrupt */
	initSPIB_SRU();
	setupSPIB();
	adi_int_InstallHandler(ADI_CID_P18I, SPIBISR, NULL, true);

	/* init codec and install interrupt */
	initDAI();
	initCS4272();
	initSPORT();
	adi_int_InstallHandler(ADI_CID_P3I, TalkThroughISR, NULL, true);

	/* audio processing and SPI communication here, upgrade flag will break the while loop */
	process_loop();

	/* delete processing algorithm here */
	release_audioBlocks();

	/* upgrade loop and SPI communication here */
	upgrade_loop();

	/* recursive function loop */
	task_loop();
}

void main()
{
	/* init PLL and external memory */
	initPLL();
	initExternalMemory();
	/* entry point of recursive loop */
	task_loop();
}
