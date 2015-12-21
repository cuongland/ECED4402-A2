/*
 * file : clock.c
 * The file is contained some fundamental for clock system
 * The main purpose is counting how many 1/10 second pass
 * Based on the number of count, doing some calculatings for coverting to hours,
 * 	minutes, seconds, and milliseconds. *
 *
 *  Created on: Oct 17, 2015
 *      Author: Duc Cuong Dinh
 */

#include "stdio.h"
#include "generalinfo.h"
#include "ControlSoftware.h"
#include "clock.h"

//global variables
//timing for the main clock
unsigned int timing_mainclock;
//timing for the stop-watch clock
unsigned int timing_stopclock;
//state of stop watch
// TRUE: working
// FALSE: not working
unsigned int state_stopwatch;

//initialization of the clock
//everything is zero and state is FALSE
void initialization_Clock(void)
{
	timing_mainclock 	 = 0;
	timing_stopclock     = 0;
	state_stopwatch	     = FALSE;
}

//The set_timing is modified the timing_mainclock to adjust the main clock
// to the current time.
//The functions is take input of hour, minute, second, and millisecond
// and converting to total 1/10 milliseconds
//No return
void set_timing(unsigned int hour, unsigned int minute, unsigned int second, unsigned int mili)
{
	timing_mainclock = (second*SECOND_TICK_TOCK) + (minute*MINUTE_TICK_TOCK) + (hour*HOUR_TICK_TOCK) + mili;
}

//When the main is call, the function will send the request to calculate how many of hours,
//minutes, seconds, and milliseconds
//no return
void get_mainclock(void)
{
	calculate_clock(timing_mainclock);
}

//When the stopwatch is working, the functions will send the request to calculate
//how many hours, minutes, seconds, and milliseconds
//no return
void get_stopclock(void)
{
	calculate_clock(timing_stopclock);
}

//the function is converting from total 1/10 milliseconds into hours, minutes,
//seconds, and milliseconds
//once it is calculated, the result will be sent to the software-control to generate
//the buffer for transmitting
void calculate_clock(unsigned int clock)
{
	unsigned int hour,minute,second,milisecond;

	hour       = clock / HOUR_TICK_TOCK;
	clock      = clock % HOUR_TICK_TOCK;

	minute     = clock / MINUTE_TICK_TOCK;
	clock      = clock % MINUTE_TICK_TOCK;

	second     = clock / SECOND_TICK_TOCK;

	milisecond = clock % SECOND_TICK_TOCK;

	get_clock(hour,minute,second,milisecond);
}

//the functions is used to update state of stop watch's state
//if the state's input is FALSE, then update directly to the variable
//if the state's input is TRUE, then update directly to the variable and clear the previous value of stop-watch
void set_stopclock_status(unsigned int status)
{
	state_stopwatch = status;
	if(status)
		timing_stopclock = 0;
}

//the function is just return the status of stopwatch
unsigned int get_stopwatch_statue(void)
{
	return state_stopwatch;
}

//when the user wants to know what time is, the function will be called
//it will request to convert from total milliseconds to hours, minutes, seconds,
//and milliseconds and sent to software-control for gerating buffer
unsigned int print_mainclock(unsigned int data)
{
	calculate_clock(timing_mainclock);
	return FALSE;
}

//when 1/10 millisecond pass, and the SysTick is fired, the function will be called
//for updating the main clock as well as the stop watch if the stop watch's state
// is TRUE
unsigned int update_clock(unsigned int data)
{
	timing_mainclock++;
	timing_mainclock = timing_mainclock % DAY_TICK_TOCK;
	if(state_stopwatch)
	{
		//printf("in here\n");
		timing_stopclock++;
		timing_stopclock = timing_stopclock % DAY_TICK_TOCK;
		calculate_clock(timing_stopclock);
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//testing sections

//print out the clock in milliseocnd
void print_clock_value(void)
{
	printf("clock: %d\n",timing_mainclock);
}

//testing how the code convert from total millisecond to hours/ minutes, seconds
//milliseconds
void calculate_clockk(void)
{
	unsigned int hour,minute,second,milisecond;
	unsigned int clock = timing_mainclock;

	hour       = clock / HOUR_TICK_TOCK;
	clock      = clock % HOUR_TICK_TOCK;

	minute     = clock / MINUTE_TICK_TOCK;
	clock      = clock % MINUTE_TICK_TOCK;

	second     = clock / SECOND_TICK_TOCK;

	milisecond = clock % SECOND_TICK_TOCK;

	printf("(hour: %d) (minute: %d) (second: %d) (millisecond: %d)\n", hour, minute, second, milisecond);

}

//testing how the code convert from hour, minutes, second, and millisecond to
// total 1/10 second
void set_timing_test(unsigned int hour, unsigned int minute, unsigned int second, unsigned int mili)
{
	timing_mainclock = (second*SECOND_TICK_TOCK) + (minute*MINUTE_TICK_TOCK) + (hour*HOUR_TICK_TOCK) + mili;
}
