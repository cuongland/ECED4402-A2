/*
 * systick.c
 *
 *  Created on: Oct 14, 2015
 *      Author: Duc Cuong Dinh
 */
#include <stdio.h>
#include <stdlib.h>

#include "systick.h"
#include "generalinfo.h"

#define testing

//external function
extern unsigned int buffer_ready_TX_wait(unsigned int data);
extern unsigned int update_clock(unsigned int data);
extern unsigned int enqueue(unsigned int data, function_pointer func);

//initialization of Systick
//input: 0->16777216 for RELOAD register
//no output
void SysTickInit(unsigned int Period)
{
	// Must be between 0 and 16777216
	SYSTICK_RELOAD_VALUE = Period - 1;
	// Set the interrupt bit in STCTRL
	// Set the clock source to internal and enable the counter
	SYSTICK_CONTROL_N_STATUS |= SYSTICK_INTERRUPT_FLAG|SYSTICK_CLOCK_SOURCE_FLAG | SYSTICK_ENABLE_FLAG;
}

//interrupt event
void SysTickHandler(void)
{
#ifdef testing
	printf("SysTick\n");
#endif
	//diable global interrupt
	DISABLE_GLOBAL_INTERRUPT();
	//add to queue: updata the clock
	//generate stop watch information of the state of stopwatch is TRUE
	enqueue(0,update_clock);
	//add to queue: waiting the buffer_TX is NULL
	enqueue(0,buffer_ready_TX_wait);
	//enable the global interrup
	ENABLE_GLOBAL_INTERRUPT();
}
