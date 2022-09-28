/**
 * \file    delay.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	09/28/2022
 * \brief   Macros for controlling approximate delays in real-time
 */

#ifndef DELAY_H_
#define DELAY_H_

/**
 * \def CLK_FREQ_IN_HZ
 *  The operating frequency in Hz
 */
#define CLK_FREQ_IN_HZ\
	(48000000/5)

/**
 * \def CLK_CYCLES_PER_ITERATION
 *  Gathered by viewing the disassembly of the wait loop and seeing how many assembly instructions are executed per iteration.
 *  Then referencing how many clock cycles each of those instructions are.
 *  Lastly, adding them up.
 *  	- ldr = 2 clock cycles
 *  	- nop = 1 clock cycle
 */
#define CLK_CYCLES_PER_ITERATION\
	(2 + 1)

/**
 * \def ITERATIONS_FOR_1_SEC
 *  The approximate amount of iterations in the wait loop in order to wait for 1 s
 */
#define ITERATIONS_FOR_1_SEC\
	(CLK_FREQ_IN_HZ/CLK_CYCLES_PER_ITERATION)

/**
 * \def ITERATIONS_FOR_100_MSEC
 *  The approximate amount of iterations in the wait loop in order to wait for 100 ms
 */
#define ITERATIONS_FOR_100_MSEC\
	(ITERATIONS_FOR_1_SEC/10)

/**
 * \def DELAY_100_MSEC()
 *  Invokes the wait loop to wait for approximately 100 ms
 */
#define DELAY_100_MSEC()\
	do{\
		for(int i_delay = 0; i_delay < ITERATIONS_FOR_100_MSEC; i_delay++){\
			__asm volatile ("nop");\
		}\
	}while(0)

#endif /* DELAY_H_ */
