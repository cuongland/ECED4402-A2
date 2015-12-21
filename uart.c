/*
 * uart.c
 *
 *  Created on: Oct 17, 2015
 *      Author: Duc Cuong Dinh
 */

/*
 * UART test code:
 * Should echo input from the keyboard on the UART console
 * Caveat emptor!
 * 30 Jan 2013
 *
 * This code works and echoes characters.  Neither the FIFOs nor the Receive-Timer interrupt
 * are used.  This makes the UART function as a run-of-the-mill one-byte-one-interrupt UART.
 * 4 Feb 2013
 */
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"
#include "ControlSoftware.h"
#include "ACriticalRegion.h"

//#define testing_uart

/* Globals */
volatile char Data_TX;  	/* Input data from UART receive */
volatile char Data_RX;  	/* Output data from UART receive */
extern volatile char *buffer_TX;

void SetupPIOSC(void)
{
/* Set BYPASS, clear USRSYSDIV and SYSDIV */
SYSCTRL_RCC_R = (SYSCTRL_RCC_R & CLEAR_USRSYSDIV) | SET_BYPASS ;	// Sets clock to PIOSC (= 16 MHz)
}

void initialization_uart(void)
{
/* Initialize UART */
SetupPIOSC();			// Set Clock Frequency to 16MHz (PIOSC)
UART0_Init();			// Initialize UART0
IntEnable(INT_UART0);		// Enable UART0 interrupts
UART0_IntEnable(UART_INT_RX | UART_INT_TX);	// Enable Receive and Transmit interrupts
}

void UART0_Init(void)
{
/* Initialize UART0 */
SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; 	// Enable Clock Gating for UART0
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; 	// Enable Clock Gating for PORTA

UART0_CTL_R &= ~UART_CTL_UARTEN;      	// Disable the UART

// Setup the BAUD rate
UART0_IBRD_R = 8;	// IBRD = int(16,000,000 / (16 * 115,200)) = int(8.68055)
UART0_FBRD_R = 44;	// FBRD = int(0.68055 * 64 + 0.5) = 44.0552
UART0_LCRH_R = (UART_LCRH_WLEN_8);	// WLEN: 8, no parity, one stop bit, without FIFOs)

UART0_CTL_R |= UART_CTL_UARTEN;        // Enable the UART and End of Transmission Interrupts

GPIO_PORTA_AFSEL_R |= EN_RX_PA0 | EN_TX_PA1;    	// Enable Receive and Transmit on PA1-0
GPIO_PORTA_DEN_R |= EN_DIG_PA0 | EN_DIG_PA1;   		// Enable Digital I/O on PA1-0
}

void IntEnable(unsigned long InterruptIndex)
{
/* Indicate to CPU which device is to interrupt */
if(InterruptIndex < 32)
	NVIC_EN0_R = 1 << InterruptIndex;		// Enable the interrupt in the EN0 Register
else
	NVIC_EN1_R = 1 << (InterruptIndex - 32);	// Enable the interrupt in the EN1 Register
}

void UART0_IntEnable(unsigned long flags)
{
/* Set specified bits for interrupt */
UART0_IM_R |= flags;
}

void set_transmitting_char(char c)
{
	UART0_DR_R = c;
}

void UART0_IntHandler(void)
{
/*
 * Simplified UART ISR - handles receive and xmit interrupts
 * Application signalled when data received
 */
	DISABLE_GLOBAL_INTERRUPT();
	if (UART0_MIS_R & UART_INT_RX)
	{
		/* RECV done - clear interrupt and make char available to application */
		UART0_ICR_R |= UART_INT_RX;
		Data_RX = (char)UART0_DR_R;
		enqueue(Data_RX,get_received_char);
		enqueue(0,buffer_ready_TX_wait);

#ifdef testing_uart
	printf("char received : %c\n", Data_RX);
#endif
	}
	ENABLE_GLOBAL_INTERRUPT();
	if (UART0_MIS_R & UART_INT_TX)
	{
		/* XMIT done - clear interrupt */
		UART0_ICR_R |= UART_INT_TX;
	}
}

