/*
 * ControlSoftware.c
 *
 *  Created on: Oct 14, 2015
 *      Author: Duc Cuong Dinh
 */
#include <stdio.h>
#include <stdlib.h>

#include "ACriticalRegion.h"
#include "ControlSoftware.h"
#include "memory.h"
#include "uart.h"
#include "cursor.h"
#include "clock.h"

#define testing_cs

//global variables
//buffer for transmit: the current transmitting
volatile char *buffer_TX;
//buffer for transmit: the ready for transmitting
volatile char *buffer_ready_TX;
//the buffer length of buffer_ready
volatile unsigned int buffer_length;

//command state:
volatile enum COMMAND_STATE current_state;

//other information:
//infmation for clock:
volatile unsigned int hour;
volatile unsigned int minute;
volatile unsigned int second;
volatile unsigned int milisecond;
//how many char is pressed
volatile unsigned int char_input;
//when enter is pressed
volatile unsigned int enter_key;

//inialization for Control Software
void initialization_Control_Software()
{
	buffer_TX  = NULL;
	buffer_ready_TX = NULL;
	buffer_ready_TX = NULL;
	current_state = VALID;
	hour = 0;
	minute = 0;
	second = 0;
	milisecond = 0;
	char_input = 0;
	enter_key = 1;
}

//for testing
//print out the every single char in buffer ready to make sure everything is ready
void print_st(void)
{
	unsigned int i;
	for(i=0;i<buffer_length;i++)
		printf("(%d %c)",i,buffer_ready_TX[i]);
	printf("\n");
}

//get_received_char:
//when a character is press, the command and the char is stored in the queue
//when dequeueing the queue, the command and the data will be burst
//it is considered whether it is valid or invalid by sequence typing
//the invalid state will be reset when enter key is hit
unsigned int get_received_char(unsigned int c)
{
	char_input++;
	if(get_stopwatch_statue())
	{
		set_stopclock_status(FALSE);
		enter_key++;
	}
	if(buffer_ready_TX == NULL)
	{
		buffer_ready_TX = (volatile char*) allocate(STRING_SIZE);
		buffer_length = 0;
	}
	switch(c)
	{
		case '\r':
			enter_key = (enter_key % ENTER_ROW) + 1;
			cursor_position(buffer_ready_TX,&buffer_length,enter_key,1);
			cursor_erase(buffer_ready_TX,&buffer_length);
			switch(current_state)
			{
				case STATE_O_2:
					set_stopclock_status(TRUE);
					//print_st();
					break;
				case STATE_E_4:
					get_mainclock();
					enter_key++;
					//print_st();
					break;
				case STATE_MIL_1:
					set_timing(hour,minute,second,milisecond);
					//print_st();
					break;
				default:
					break;
			}
			char_input = 0;
			current_state = VALID;
			break;
		case ' ':
			current_state = (current_state == STATE_T_3) ? STATE_SPACE_4 : INVALID;
			break;
		case '0':
		case '1':
		case '2':
			switch(current_state)
			{
				case STATE_SPACE_4:
					current_state = STATE_HOUR_1;
					hour = (c - '0') * BASE_NUMBER_10;
					break;
				case STATE_HOUR_1:
					current_state = STATE_HOUR_2;
					hour += (c - '0');
					break;
				case STATE_HOUR_2:
					current_state = STATE_MIN_1;
					minute = (c - '0') * BASE_NUMBER_10;
					break;
				case STATE_MIN_1:
					current_state = STATE_MIN_2;
					minute += (c - '0');
					break;
				case STATE_MIN_2:
					current_state = STATE_SEC_1;
					second = (c - '0') * BASE_NUMBER_10;
					break;
				case STATE_SEC_1:
					current_state = STATE_SEC_2;
					second += (c - '0');
					break;
				case STATE_SEC_2:
					current_state = STATE_MIL_1;
					milisecond = (c - '0');
					break;
				default:
					current_state = INVALID;
					break;
			}
			break;
		case '3':
		case '4':
		case '5':
			switch(current_state)
			{
				case STATE_HOUR_1:
					hour += (c - '0');
					current_state = (hour < MAX_HOUR) ? STATE_HOUR_2 : INVALID;
					break;
				case STATE_HOUR_2:
					current_state = STATE_MIN_1;
					minute = (c - '0') * BASE_NUMBER_10;
					break;
				case STATE_MIN_1:
					current_state = STATE_MIN_2;
					minute += (c - '0');
					break;
				case STATE_MIN_2:
					current_state = STATE_SEC_1;
					second = (c - '0') * BASE_NUMBER_10;
					break;
				case STATE_SEC_1:
					current_state = STATE_SEC_2;
					second += (c - '0');
					break;
				case STATE_SEC_2:
					current_state = STATE_MIL_1;
					milisecond = (c - '0');
					break;
				default:
					current_state = INVALID;
					break;
			}
			break;
		case '6':
		case '7':
		case '8':
		case '9':
			switch(current_state)
			{
				case STATE_HOUR_1:
					hour += (c - '0');
					current_state = (hour <MAX_HOUR) ? STATE_HOUR_2 : INVALID;
					break;
				case STATE_MIN_1:
					minute += (c - '0');
					current_state = (minute < MAX_MINUTE) ? STATE_MIN_2 : INVALID;
					break;
				case STATE_SEC_1:
					second += (c - '0');
					current_state = (second < MAX_SECOND) ? STATE_SEC_2 : INVALID;
					break;
				case STATE_SEC_2:
					current_state = STATE_MIL_1;
					milisecond = (c - '0');
					break;
				default:
					current_state = INVALID;
					break;
			}
			break;
		case 'E':
			current_state = (current_state == STATE_M_3) ? STATE_E_4 : ((current_state == STATE_S_1) ? STATE_E_2 : INVALID);
			break;
		case 'G':
			current_state = (current_state == VALID) ? STATE_G_1 : INVALID;
			break;
		case 'I':
			current_state = (current_state == STATE_T_1) ? STATE_I_2 : INVALID;
			break;
		case 'M':
			current_state = (current_state == STATE_I_2) ? STATE_M_3 : INVALID;
			break;
		case 'O':
			current_state = (current_state == STATE_G_1) ? STATE_O_2 : INVALID;
			break;
		case 'S':
			current_state = (current_state == VALID) ? STATE_S_1 : INVALID;
			break;
		case 'T':
			current_state = (current_state == VALID) ? STATE_T_1 : ((current_state == STATE_E_2) ? STATE_T_3: INVALID);
			break;
		case 'e':
			current_state = (current_state == STATE_M_3) ? STATE_E_4 : ((current_state == STATE_S_1) ? STATE_E_2 : INVALID);
			break;
		case 'g':
			current_state = (current_state == VALID) ? STATE_G_1 : INVALID;
			break;
		case 'i':
			current_state = (current_state == STATE_T_1) ? STATE_I_2 : INVALID;
			break;
		case 'm':
			current_state = (current_state == STATE_I_2) ? STATE_M_3 : INVALID;
			break;
		case 'o':
			current_state = (current_state == STATE_G_1) ? STATE_O_2 : INVALID;
			break;
		case 's':
			current_state = (current_state == VALID) ? STATE_S_1 : INVALID;
			break;
		case 't':
			current_state = (current_state == VALID) ? STATE_T_1 : ((current_state == STATE_E_2) ? STATE_T_3: INVALID);
			break;
		default:
			current_state = INVALID;
			break;
	}
	generate_command_string(c);
	//printf("<<<<<buffer is ready: %c >>>>>\n",c);
#ifdef testing_cs
	printf("char receive: %c - state:", c);
	switch(current_state)
	{
		case INVALID:
			printf("INVALID\n");
			break;
		case VALID:
			printf("VALID\n");
			break;
		case STATE_G_1:
			printf("STATE_G_1\n");
			break;
		case STATE_O_2:
			printf("STATE_O_2\n");
			break;
		case STATE_T_1:
			printf("STATE_T_1\n");
			break;
		case STATE_I_2:
			printf("STATE_I_2\n");
			break;
		case STATE_M_3:
			printf("STATE_M_3\n");
			break;
		case STATE_E_4:
			printf("STATE_E_4\n");
			break;
		case STATE_S_1:
			printf("STATE_S_1\n");
			break;
		case STATE_E_2:
			printf("STATE_E_2\n");
			break;
		case STATE_T_3:
			printf("STATE_T_3\n");
			break;
		case STATE_SPACE_4:
			printf("STATE_T_3\n");
			break;
		case STATE_HOUR_1:
			printf("STATE_HOUR_1\n");
			break;
		case STATE_HOUR_2:
			printf("STATE_HOUR_2\n");
			break;
		case STATE_MIN_1:
			printf("STATE_MIN_1\n");
			break;
		case STATE_MIN_2:
			printf("STATE_MIN_2\n");
			break;
		case STATE_SEC_1:
			printf("STATE_SEC_1\n");
			break;
		case STATE_SEC_2:
			printf("STATE_SEC_2\n");
			break;
		case STATE_MIL_1:
			printf("STATE_MIL_1\n");
			break;
		default:
			printf("unknown case\n");
			break;
	}
#endif
	buffer_ready_TX[buffer_length++] = (char)NULL_INDEX;//the end of string
	//print_st();
	return FALSE;
}

//when buffer_Tx is not NULL
//it will return the char,
//when the string is reached the sepecial char (0xFF/NULL_INDEX), the return char is NUL(0x0)
// and buffer_TX will be pointed to 0x0
char get_transmitting_char(void)
{
	//getting a char
	char result = buffer_TX[0];
	//checking the char
	if(result==(char)NULL_INDEX)
	{
		//if the char is 0xFF/NULL_INDEX
		//deallocate the memory for buffer_TX
		deallocate((unsigned int *)buffer_TX);
		//Buffer_TX is pointed to NULL
		buffer_TX = (volatile char *) NULL;
		//the char is getting a new value: 0x0
		result = (char)NUL;
	}
	else
		//if the char is not 0xFF/NULL_INDEX
		//update buffer_TX: increment by one
		buffer_TX++;
	//return the value
	return result;
}

//Once the clock is calculated the total 1/10 second, it will call this function
//to through the data: hour, minute, second, millisecond
//First: the function getting the data
//Second: generating the buffer_ready_TX
void get_clock(unsigned int hour_, unsigned int minute_, unsigned int second_, unsigned int milisecond_)
{
	hour = hour_;
	minute = minute_;
	second = second_;
	milisecond = milisecond_;

	generate_clock_string();
}

//convert information of clock into string
void convert_clock_to_string(void)
{
	buffer_ready_TX[buffer_length++] = (hour / BASE_NUMBER_10) + '0';
	buffer_ready_TX[buffer_length++] = (hour % BASE_NUMBER_10) + '0';
	buffer_ready_TX[buffer_length++] = ':';
	buffer_ready_TX[buffer_length++] = (minute / BASE_NUMBER_10) + '0';
	buffer_ready_TX[buffer_length++] = (minute % BASE_NUMBER_10) + '0';
	buffer_ready_TX[buffer_length++] = ':';
	buffer_ready_TX[buffer_length++] = (second / BASE_NUMBER_10) + '0';
	buffer_ready_TX[buffer_length++] = (second % BASE_NUMBER_10) + '0';
	buffer_ready_TX[buffer_length++] = '.';
	buffer_ready_TX[buffer_length++] = milisecond + '0';
	buffer_ready_TX[buffer_length++] = '\0';
}

//generating the clock information and stored in buffer_ready_TX
void generate_clock_string(void)
{
	if(buffer_ready_TX==NULL)
	{
		buffer_ready_TX = (volatile char*) allocate(STRING_SIZE);
		buffer_length = 0;
	}
	if(buffer_ready_TX!=NULL)
	{
		//adding postion
		cursor_position(buffer_ready_TX, &buffer_length, enter_key, 1);
		//adding clock infor
		convert_clock_to_string();

		buffer_ready_TX[buffer_length++] = (char)NULL_INDEX;//the end of string
	}
}

//generating the echo of typing
void generate_command_string(char c)
{
	if(buffer_ready_TX != NULL)
	{
		if(buffer_length>0 && (buffer_ready_TX[buffer_length++] == (char)NULL_INDEX))
			buffer_length--;

		//adding postion
		cursor_position(buffer_ready_TX, &buffer_length, enter_key, char_input);
		//if the char input is not enter key
		if(c != '\r')
		{
			//add char to the string
			buffer_ready_TX[buffer_length++] = c;
			//if the current state is state_hour_2 or state_minute_2
			//then add another char ":" right after the key typed
			if(current_state==STATE_HOUR_2 || current_state==STATE_MIN_2)
			{
				char_input++;
				buffer_ready_TX[buffer_length++] = ':';
			}
			//if the current state is state_sec_2
			if(current_state==STATE_SEC_2)
			{
				char_input++;
				buffer_ready_TX[buffer_length++] = '.';
			}
		}
		else
			cursor_erase(buffer_ready_TX, &buffer_length);
	}
}

//waiting until buffer_TX is NUL
//once the buffer_TX is NULL, buffer_TX get the address of buffer_ready_TX
unsigned int buffer_ready_TX_wait(unsigned int data)
{
	if(buffer_TX != (char*)NULL)
		return TRUE;

	buffer_length = 0;
	buffer_TX = buffer_ready_TX;
	buffer_ready_TX = (volatile char*) NULL;

	return FALSE;
}

//running program
void running(void)
{
	unsigned int data;
	function_pointer func_;
	while(1)
	{
		//if there is the task to do
		if(dequeue(&data,&func_))
		{
			//running the tast until it hit FALSE
			while(func_(data))
			{
				//transmit char
				if(buffer_TX != (volatile char *)NULL)
				{
					if((UART0_FR_R & UART_FR_BUSY)==0)
						set_transmitting_char(get_transmitting_char());
				}
			}
		}
		//transmit char
		if(buffer_TX != (volatile char *)NULL)
		{
			if((UART0_FR_R & UART_FR_BUSY)==0)
				set_transmitting_char(get_transmitting_char());
		}
	}
}
