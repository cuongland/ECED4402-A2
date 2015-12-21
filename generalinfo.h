/*	FILE: generalinfo.h
 * 	the file is stored everything general information or macro functions
 *
 * 	Date: September 20, 2015
 * 	Authors: Duc Cuong Dinh (B00516935)
 * 	         Jie Zhang      (B00583787)
 *
 */

#ifndef GENERALINFO_H_
#define GENERALINFO_H_
/*
 *  adding library
 */


/*
 *  constant values
 */
#define NUL					0x0
#define TRUE    			1
#define FALSE   			0
#define ESC					0x1B
#define BASE_NUMBER_10		10

typedef unsigned int (*function_pointer)(unsigned int a);


#define BITBAND_ALIAS_BASE   0x22000000
#define BITBAND_ALIAS_ADDRESS(x, y) ((unsigned int *) ( (((unsigned int)(x))<<5) + ((y)<<2) + BITBAND_ALIAS_BASE))

//ASSEMBLY INSTRUCTION
#define ENABLE_GLOBAL_INTERRUPT() 	__asm(" cpsie	i");
#define DISABLE_GLOBAL_INTERRUPT() 	__asm(" cpsid	i");

#endif /* GENERALINFO_H_ */
