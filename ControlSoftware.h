/*
 * ControlSoftware.h
 *
 *  Created on: Oct 14, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef CONTROLSOFTWARE_H_
#define CONTROLSOFTWARE_H_

#define STRING_SIZE		128
#define ENTER_ROW		23

enum COMMAND_STATE {INVALID, VALID,
					//command GO
					STATE_G_1, STATE_O_2,
					//command TIME
					STATE_T_1, STATE_I_2, STATE_M_3, STATE_E_4,
					//command SET 00:00:00.0
					STATE_S_1, STATE_E_2, STATE_T_3, STATE_SPACE_4,STATE_HOUR_1,
					STATE_HOUR_2, STATE_MIN_1, STATE_MIN_2, STATE_SEC_1,
					STATE_SEC_2, STATE_MIL_1};

void convert_clock_to_string(void);
void generate_command_string(char c);
void get_clock(unsigned int hour_, unsigned int minute_, unsigned int second_, unsigned int milisecond_);
unsigned int get_received_char(unsigned int c);
void running(void);
void initialization_Control_Software();
char get_transmitting_char(void);
void generate_clock_string(void);
unsigned int buffer_ready_TX_wait(unsigned int data);
void buffer_TX_select(void);

#endif /* CONTROLSOFTWARE_H_ */
