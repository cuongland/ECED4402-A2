/*
 * main.c
 */
#include <stdio.h>

#include "generalinfo.h"
#include "ControlSoftware.h"
#include "ACriticalRegion.h"
#include "systick.h"
#include "clock.h"
#include "memory.h"
#include "uart.h"

#include "testcases.h"

extern volatile char *buffer_ready_TX;
extern unsigned int buffer_ready_TX_wait(unsigned int data);
extern unsigned int update_clock(unsigned int data);
extern unsigned int enqueue(unsigned int data, function_pointer func);


void init(void)
{
	initial_memory();
	initialization_Clock();
	initialize_ACriticalRegion();
	SysTickInit(1600000);
	initialization_uart();
	initialization_Control_Software();
}

void run(void)
{
	init();
	ENABLE_GLOBAL_INTERRUPT();
	running();
}

int main(void)
{
	run();
	return 0;
}
