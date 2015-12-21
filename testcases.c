/*
 * testcases.c
 *
 *  Created on: Oct 22, 2015
 *      Author: Duc Cuong Dinh
 */

#include <stdio.h>

#include "generalinfo.h"
#include "clock.h"
#include "AcriticalRegion.h"
#include "systick.h"
#include "memory.h"
#include "ControlSoftware.h"
#include "uart.h"

extern unsigned int update_clock(unsigned int data);
//clock handling cases
//checking input HOUR, MINUTE, SECOND, milisecond

void testcase1(void)
{
	//initializing the clock system
	initialization_Clock();

	//input 1
	set_timing(0,0,0,0);
	//actual result
	print_clock_value();
	calculate_clockk();
	//input 2
	set_timing(23,59,59,9);
	//actual result
	print_clock_value();
	calculate_clockk();
	//input 3
	set_timing(12,00,00,0);
	//actual result
	print_clock_value();
	calculate_clockk();
}

void testcase2(void)
{
	//initializing the clock system
	initialization_Clock();
	//input 1
	set_timing(0,0,0,0);
	//actual result
	print_clock_value();
	update_clock(1);
	calculate_clockk();
	//input 2
	set_timing(23,59,59,9);
	//actual result
	print_clock_value();
	update_clock(1);
	calculate_clockk();
	//input 3
	set_timing(00,59,59,9);
	//actual result
	print_clock_value();
	update_clock(1);
	calculate_clockk();
	//input 4
	set_timing(9,59,59,9);
	//actual result
	print_clock_value();
	update_clock(1);
	update_clock(1);
	calculate_clockk();
}

//queue
void testcase3(void)
{
	unsigned int count;

	//init
	initialize_ACriticalRegion();

	for(count=0;count<MAXINUMQUEUES+2;count++)
	{
		printf("enqueue %d : ",count);
		if(enqueue(1,update_clock))
			printf("success\n");
		else
			printf("fail\n");
	}
}

void testcase4(void)
{
	//init
	initialize_ACriticalRegion();
	//setup
	unsigned int data;
	unsigned int count;
	function_pointer func_;
	//setup for queue
	for(count=0;count<MAXINUMQUEUES;count++)
	{
		printf("enqueue %d : ",count);
		if(enqueue((count+1),update_clock))
			printf("success\n");
		else
			printf("fail\n");
	}
	//dequeue
	for(count=0;count<MAXINUMQUEUES;count++)
	{
		printf("dequeue %d : ",count);
		if(dequeue(&data,&func_))
			printf("success: (data: %d) (func: %08x)\n", data,(unsigned int)func_);
		else
			printf("fail\n");
	}

	printf("dequeue %d : ",count);
	if(dequeue(&data,&func_))
		printf("success: (data: %d) (func: %08x)\n", data,(unsigned int)func_);
	else
		printf("fail\n");

}

//systick
void testcase5(void)
{
	SysTickInit(1000000);
	ENABLE_GLOBAL_INTERRUPT();
	printf("If the Systick occurs, it will print out SYSTICK everytime it's fired.");
	while(1);
}

//uart
void testcase6()
{
	initial_memory();
	initialize_ACriticalRegion();
	initialization_Control_Software();
	initialization_uart();
	ENABLE_GLOBAL_INTERRUPT();
	while(1);
}

void testcase7()
{
	initial_memory();
	initialization_uart();
	initialization_Control_Software();
	initialization_uart();
	ENABLE_GLOBAL_INTERRUPT();
	while(1);
}

void testcase8(void)
{
	initial_memory();
	initialization_uart();
	initialization_Control_Software();
	initialization_uart();
	ENABLE_GLOBAL_INTERRUPT();
	while(1);
}
