/*
 * clock.h
 *
 *  Created on: Oct 17, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#define DAY_TICK_TOCK		864000
#define HOUR_TICK_TOCK		36000
#define MINUTE_TICK_TOCK	600
#define SECOND_TICK_TOCK	10

#define MAX_HOUR			24
#define MAX_MINUTE			60
#define	MAX_SECOND			60
#define	MAX_MILISECOND		10

//The set_timing is modified the timing_mainclock to adjust the main clock
// to the current time.
//The functions is take input of hour, minute, second, and millisecond
// and converting to total 1/10 milliseconds
//No return
void set_timing(unsigned int hour, unsigned int minute, unsigned int second, unsigned int mili);

unsigned int get_stopwatch_statue(void);

//when 1/10 millisecond pass, and the SysTick is fired, the function will be called
//for updating the main clock as well as the stop watch if the stop watch's state
// is TRUE
unsigned int update_clock(unsigned int data);

//the function is converting from total 1/10 milliseconds into hours, minutes,
//seconds, and milliseconds
//once it is calculated, the result will be sent to the software-control to generate
//the buffer for transmitting
void calculate_clock(unsigned int clock);

//When the stopwatch is working, the functions will send the request to calculate
//how many hours, minutes, seconds, and milliseconds
//no return
void get_stopclock(void);

//When the main is call, the function will send the request to calculate how many of hours,
//minutes, seconds, and milliseconds
//no return
void get_mainclock(void);

//initialization of the clock
//everything is zero and state is FALSE
void initialization_Clock(void);

//the functions is used to update state of stop watch's state
//if the state's input is FALSE, then update directly to the variable
//if the state's input is TRUE, then update directly to the variable and clear the previous value of stop-watch
void set_stopclock_status(unsigned int status);

//when the user wants to know what time is, the function will be called
//it will request to convert from total milliseconds to hours, minutes, seconds,
//and milliseconds and sent to software-control for gerating buffer
unsigned int print_mainclock(unsigned int data);

////////////////////////////////////////////////////////////////////////////////
//testing sections

void calculate_clockk(void);
void set_timing_test(unsigned int hour, unsigned int minute, unsigned int second, unsigned int mili);
void print_clock_value(void);
#endif /* CLOCK_H_ */
