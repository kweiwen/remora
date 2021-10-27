#include "commandProcess.h"
#include "Driver/UART0_isr.h"
#include "Driver/ADDS_21479_EzKit.h"

void handleCommandData()
{
	commandReady = 0;
	isCommandProcessing = 1;

	/** place handle command here! */
	DBG(uart_buffer);

	isCommandProcessing = 0;
}
