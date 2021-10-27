#include <cdef21479.h>
#include <def21479.h>
#include <stdio.h>					/* Get declaration of puts and definition of NULL. */
#include <assert.h>    				/* Get the definition of support for standard C asserts. */
#include <services/int/adi_int.h>  	/* Interrupt Handler API header. */

#include "UART0_isr.h"
#include "ADDS_21479_EzKit.h"

char uart_buffer[UART_BUFFER_SIZE] = {0};
volatile int uart_buffer_cntr = 0;
volatile int uart_buffer_ready = 0;
volatile int timer_isr_count = 0;

char welcomeMessage[] =
{
		"\f"
		"Welcome!"
		"\n"
		"Copyright (c) 2021 Lava Music, Inc.  All rights reserved."
		" This software is proprietary and confidential.  By using this software you agree"
		" to the terms of the associated Lava Music License Agreement.  Enjoy!"
		"\n"
};

char errorMessage[] =
{
		"\f"
		"ERROR!"
		"\n"
		"UART controller is not able to collect enough data in time,"
		" message buffer will be clear."
		"\n"
};

void initUART(void)
{
   	*pPICR2 &= ~(0x7C00); /* Sets the UART0 receive interrupt to P14 */
   	*pPICR2 |= (0x13<<10);

   	*pUART0LCR=0;

    *pUART0IER= UARTRBFIE;  /* enables UART0 receive interrupt */

	/* Sets the Baud rate for UART0 */
	*pUART0LCR = UARTDLAB;  /* enables access to Divisor register to set baud rate */

	/* Baud rate = PCLK/(16 * divisor), divisor value can be from 1 to 65,536 */
	/* 133Mhz = PCLK */
	/* 0x01B0 = 432 for divisor value and gives a baud rate of 19200 for core clock 266MHz */
	/* 0x0048 = 72 for divisor value and gives a baud rate of 115200 for core clock 266MHz */
	/* 0x000E = 14 for divisor value and gives a baud rate of 576000 for core clock 266MHz */
	*pUART0DLL = 0x0E;
    *pUART0DLH = 0x00;

    /* Configures UART0 LCR */
    /* word length 8, parity enable ODD parity, Two stop bits */
    *pUART0LCR = UARTWLS8 | UARTPEN | UARTSTB;

    *pUART0RXCTL = UARTEN;       /* enables UART0 in receive mode */
    *pUART0TXCTL = UARTEN;       /* enables UART0 in core driven mode */

    DBG(welcomeMessage);
}

void xmitUARTmessage(char*xmit, int SIZE)
{
	int i;

	/* loop to transmit source array in core driven mode */
	for (i=0; i<SIZE; i++)
	{
		/* Wait for the UART transmitter to be ready */
		while ((*pUART0LSR & UARTTHRE) == 0)
		{
			/* Loop until condition */
			asm("nop;");
		}

		/* Transmit a byte */
		*pUART0THR = xmit[i];
	}

	/* poll to ensure UART has completed the transfer */
	while ((*pUART0LSR & UARTTEMT) == 0)
	{
		/* Loop until condition */
		asm("nop;");
	}
}

#pragma optimize_for_speed /* interrupt handlers usually need to be optimized */
#pragma section ("seg_int_code")  /* handler functions perform better in internal memory */
void UART0ISR(uint32_t iid, void *handlerArg)
{
	/* UART interrupt service routine. */

	/* Don't call standard I/O functions or update
	 * non-volatile global data in interrupt handlers.
	 */

	/* You can use the handler arguments to
	 * identify the interrupt being handled (iid)
	 * and to access data via an interrupt specific
	 * callback pointer argument (handlerArg).
	 */
	assert(iid == ADI_CID_P14I);
	assert(handlerArg == NULL);

	int value;
	if(*pUART0LSR & UARTDR)
	{
		/* clean buffer at the first time, invert the status logic to prevent re-write */
		if(uart_buffer_ready==0)
		{
			for(int i=0; i<UART_BUFFER_SIZE; i++)
			{
				uart_buffer[i] = 0;
			}
			uart_buffer_ready=1;
		}

		/* assign register value to variable and store in the buffer array */
		value = *pUART0RBR;
		uart_buffer[uart_buffer_cntr++] = value;
		uart_buffer_cntr &= UART_BUFFER_MASK;

		/* once the buffer is full transmit the message to UART TX */
		if(uart_buffer_cntr==0)
		{
			timer_off();
			uart_buffer_ready = 0;
			timer_isr_count = 0;
			xmitUARTmessage(uart_buffer, UART_BUFFER_SIZE);
		}
		else
		{
			/* Set tperiod and tcount of the timer. */
			timer_set(1000, 1000);
			/* Start the timer. */
			timer_on();
		}
	}
} /* UART0ISR */

#pragma optimize_for_speed /* interrupt handlers usually need to be optimized */
#pragma section ("seg_int_code")  /* handler functions perform better in internal memory */
void TimerISR(uint32_t iid, void* handlerArg)
{
	/* Timer interrupt service routine. */

	/* Don't call standard I/O functions or update
	 * non-volatile global data in interrupt handlers.
	 */

	/* You can use the handler arguments to
	 * identify the interrupt being handled (iid)
	 * and to access data via an interrupt specific
	 * callback pointer argument (handlerArg).
	 */
	assert(iid == ADI_CID_TMZHI || iid == ADI_CID_TMZLI);
	assert(handlerArg != NULL);

	volatile int *timer_counter = (int *) handlerArg;
	*timer_counter += 1;

	if(*timer_counter > 4096)
	{
		timer_off();
		uart_buffer_ready = 0;
		uart_buffer_cntr = 0;
		timer_isr_count = 0;
		/* we should add an error flag here */
	}

} /* TimerISR */

void DBG(char* input_data)
{
	int index = 0;

	while(input_data[index] != '\0')
	{
		index++;
	}
	xmitUARTmessage(input_data, index+1);
}
