/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    Blinkenlights.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/* Referenced from Alexander G Dean */
#define MASK(x)						((uint32_t)(1UL << (x)))
#define PCR_MUX_SEL_GPIO			(1)
#define PORTB_RED_LED_PIN			(18)
#define PORTB_GRN_LED_PIN			(19)
#define PORTD_BLU_LED_PIN			(1)
#define LED_ON(x)					do{\
										switch(x){\
											case white:\
												(PTB->PCOR |= MASK(PORTB_RED_LED_PIN));\
												(PTB->PCOR |= MASK(PORTB_GRN_LED_PIN));\
												(PTD->PCOR |= MASK(PORTD_BLU_LED_PIN));\
												break;\
											case red:\
												(PTB->PCOR |= MASK(PORTB_RED_LED_PIN));\
												break;\
											case green:\
												(PTB->PCOR |= MASK(PORTB_GRN_LED_PIN));\
												break;\
											case blue:\
												(PTD->PCOR |= MASK(PORTD_BLU_LED_PIN));\
												break;\
											default:\
												break;\
										}\
									}while(0)
#define LED_OFF(x)					do{\
										switch(x){\
											case white:\
												(PTB->PSOR |= MASK(PORTB_RED_LED_PIN));\
												(PTB->PSOR |= MASK(PORTB_GRN_LED_PIN));\
												(PTD->PSOR |= MASK(PORTD_BLU_LED_PIN));\
												break;\
											case red:\
												(PTB->PSOR |= MASK(PORTB_RED_LED_PIN));\
												break;\
											case green:\
												(PTB->PSOR |= MASK(PORTB_GRN_LED_PIN));\
												break;\
											case blue:\
												(PTD->PSOR |= MASK(PORTD_BLU_LED_PIN));\
												break;\
											default:\
												break;\
										}\
									}while(0)
#define LED_TOGGLE(x)				do{\
										switch(x){\
											case white:\
												(PTB->PTOR |= MASK(PORTB_RED_LED_PIN));\
												(PTB->PTOR |= MASK(PORTB_GRN_LED_PIN));\
												(PTD->PTOR |= MASK(PORTD_BLU_LED_PIN));\
												break;\
											case red:\
												(PTB->PTOR |= MASK(PORTB_RED_LED_PIN));\
												break;\
											case green:\
												(PTB->PTOR |= MASK(PORTB_GRN_LED_PIN));\
												break;\
											case blue:\
												(PTD->PTOR |= MASK(PORTD_BLU_LED_PIN));\
												break;\
											default:\
												break;\
										}\
									}while(0)

#define TSI0_MODE_NON_NOISE			(0UL)
#define TSI0_REFCHRG_500_NANO_AMP	(0UL)
#define TSI0_DVOLT_DEFAULT			(0UL)
#define TSI0_EXTCHRG_500_NANO_AMP	(0UL)
#define TSI0_PS_CLK_FREQ_DIV_BY_1	(0UL)
#define TSI0_NSCN_32_TIMES			(31UL)
#define TSI0_CHANNEL_10				(10UL)
#define TOUCH_OFFSET				(700)
#define TOUCH_DATA 					(TSI0->DATA & 0xFFFF)
#define TOUCH_UNTOUCHED_MAX			(100)
#define TOUCH_LEFT_MAX				(500)
#define TOUCH_RIGHT_MIN				(2000)
#define GET_TOUCH()					do{\
										/**
										 * Select TSI0 channel 10
										 */\
										TSI0->DATA = TSI_DATA_TSICH(TSI0_CHANNEL_10);\
								    	/**
								    	 * Software trigger to start scan
								    	 */\
										TSI0->DATA |= TSI_DATA_SWTS_MASK;\
								    	/**
								    	 * Wait for scan to complete 32 times
								    	 */\
										while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK));\
								    	/**
								    	 * Now that scan has completed 32 times, store the data and subtract TOUCH_OFFSET
								    	 */\
										scanned_value = TOUCH_DATA - TOUCH_OFFSET;\
								    	/**
								    	 * Clear the end-of-scan flag
								    	 */\
										TSI0->GENCS |= TSI_GENCS_EOSF_MASK;\
									}while(0)
#define GET_LED_COLOR()				do{\
										onboard_led_prev = onboard_led;\
										\
										if(scanned_value < TOUCH_UNTOUCHED_MAX && onboard_led_prev == white){\
											onboard_led = white;\
										}\
										else if(scanned_value < TOUCH_LEFT_MAX){\
											onboard_led = red;\
										}\
										else if(scanned_value < TOUCH_RIGHT_MIN){\
											onboard_led = green;\
										}\
										else{\
											onboard_led = blue;\
										}\
										\
										if(onboard_led != onboard_led_prev){\
											LED_OFF(onboard_led_prev);\
											LED_ON(onboard_led);\
										}\
										\
										PRINTF("Scanned value = %d\r\n", scanned_value);\
									}while(0)

#define CLK_FREQ_IN_HZ				(48000000/5)
#define CLK_CYCLES_PER_ITERATION	(3)
#define ITERATIONS_FOR_1_SEC		(CLK_FREQ_IN_HZ/CLK_CYCLES_PER_ITERATION)
#define ITERATIONS_FOR_500_MSEC		(ITERATIONS_FOR_1_SEC/2)
#define ITERATIONS_FOR_100_MSEC		(ITERATIONS_FOR_1_SEC/10)
#define DELAY_1_SEC()				do{\
										i = 0;\
										while((i++) < ITERATIONS_FOR_1_SEC){\
											__asm volatile ("nop");\
										}\
										i = 0;\
									}while(0)
#define DELAY_500_MSEC()			do{\
										i = 0;\
										while((i++) < ITERATIONS_FOR_500_MSEC){\
											__asm volatile ("nop");\
										}\
										i = 0;\
									}while(0)
#define DELAY_100_MSEC()			do{\
										i = 0;\
										while((i++) < ITERATIONS_FOR_100_MSEC){\
											__asm volatile ("nop");\
										}\
										i = 0;\
									}while(0)

typedef enum {
	white,
	red,
	green,
	blue
} color;

/**
 * \fn void init_onboard_leds
 * \brief Initialize all 3 on-board LEDs as GPIO outputs and turn them all off. Referenced operations from https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_2/Source/main.c
 * \param N/A
 * \return N/A
 *
 * \detail Many operations were referenced from Alexander G Dean (Chapter 2 of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
 * 		SIM:	System Integration Module is a peripheral containing many control registers, including SCGC5
 * 		SCGC5:	System Clock Gating Control Register 5 is a register containing different controls, including clock gating for Ports A-E (controlled with bits 9:13 respectively)
 * 		PORTB:	Port containing 32 pins, including red + green on-board LEDs
 * 		PORTD:	Port containing 32 pins, including blue on-board LED
 * 		PCR:	Pin Control Register, though in this function PCR is an array of 32 PCR registers (where each Port A-E has its own array of PCR registers). All 32 pins within each Port A-E have their own PCR register (e.g. pin 1 on Port A has its own 32-bit PCR)
 * 		PTB:	Points to all GPIO Control Registers for Port B, including PDDR
 * 		PTD:	Points to all GPIO Control Registers for Port D, including PDDR
 * 		PDDR:	Pin Data Direction Register is a register allowing configuration of a pin on the respective port A-E (where 0 is input, 1 is output)
 * 		PCOR:	Port Clear Output Register is a register used to clear some bits on the respective port A-E (e.g. writing 0x00000070 to PTA->PCOR will clear bits 6:4 on Port A)
 * 		PSOR:	Port Set Output Register is a register used to set some bits on the respective port A-E (e.g. writing 0x00000070 to PTA->PSOR will set bits 6:4 on Port A)
 *		PTOR:	Port Toggle Output Register is a register used to toggle some bits on the respective port A-E (e.g. writing 0x00000070 to PTA-PTOR will toggle bits 6:4 on Port A)
 */
void init_onboard_leds(void){
    /**
     *  Force the counter to be placed into memory
     */
    volatile static int i;

	/**
     * Enable clock to Port B for red + green on-board LEDs
     * Enable clock to Port D for blue on-board LED
     */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK + SIM_SCGC5_PORTD_MASK;

    /**
     * Set PTB18 as GPIO for red on-board LED
     * Set PTB19 as GPIO for green on-board LED
     * Set PTD1 as GPIO for blue on-board LED
     *
     * The MUX selection in PCR is done with bits 10:8, where 001 is configuration as GPIO
     */
	PORTB->PCR[PORTB_RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PORTB_RED_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_SEL_GPIO);
	PORTB->PCR[PORTB_GRN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PORTB_GRN_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_SEL_GPIO);
	PORTD->PCR[PORTD_BLU_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PORTD_BLU_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_SEL_GPIO);

	/**
	 * Set PTB18 as output for red on-board LED
	 * Set PTB19 as output for green on-board LED
	 * Set PTD1 as output for blue on-board LED
	 */
    PTB->PDDR |= MASK(PORTB_RED_LED_PIN) | MASK(PORTB_GRN_LED_PIN);
    PTD->PDDR |= MASK(PORTD_BLU_LED_PIN);

    /**
     * Turn all on-board LEDs off. Note that on-board LEDs are active-low
     */
    LED_OFF(red);
    LED_OFF(green);
    LED_OFF(blue);

    /**
     * Turn red LED on for 500 msec, and then off for 100 msec
     * Turn green LED on for 500 msec, and then off for 100 msec
     * Turn blue LED on for 500 msec, and then off for 100 msec
     * Turn white LED (red + green + blue) on for 100 msec, and then off for 100 msec
     * Turn white LED (red + green + blue) on for 100 msec, and then off for 100 msec
     */
    LED_ON(red);
    DELAY_500_MSEC();
	LED_OFF(red);
	DELAY_100_MSEC();

    LED_ON(green);
	DELAY_500_MSEC();
    LED_ON(green);
	DELAY_100_MSEC();

    LED_ON(blue);
	DELAY_500_MSEC();
    LED_ON(blue);
	DELAY_100_MSEC();

	LED_ON(white);
	DELAY_100_MSEC();
	LED_OFF(white);
	DELAY_100_MSEC();

	LED_ON(white);
	DELAY_100_MSEC();
	LED_OFF(white);
	DELAY_100_MSEC();
}

 /**
  * \fn void init_onboard_touch_sensor
  * \brief Initialize capacitive touch sensor. Referenced operations from https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense
  * \param N/A
  * \return N/A
  *
  * \detail Many operations were referenced from Alexander G Dean's TSI project on GitHub (see brief for link)
  * 		TSI: 		Touch Sensing Interface refers to interfacing with the on-board capacitive touch pad
  * 		SIM:		System Integration Module is a peripheral containing many control registers, including SCGC5
  * 		SCGC5:		System Clock Gating Control Register 5 is a register containing different controls, including clock gating for TSI
  * 		TSI0:		TSI peripheral 0 (there are 16 channels for each TSI peripheral, though this board only has 1 on-board TSI peripheral)
  * 		GENCS:		General Control and Status Register for TSI module (where each channel 0-15 has its own GENCS register)
  * 		MODE:		GENCS configuration for TSI operation mode. 4 modes available:
  * 						- 0: Non-noise mode
  * 						- 1: Noise threshold detection mode
  * 						- 2: Noise threshold detection mode
  * 						- 3: Automatic noise detection mode
  * 		REFCHRG:	GENCS configuration for reference oscillator charge/discharge. 8 values available from 500 nA to 64 uA
  * 		DVOLT:		GENCS configuration for oscillator voltage rails. 4 levels available
  * 						- 0:
  * 						- 1:
  * 						- 2:
  * 						- 3:
  * 		EXCHRG:		GENCS configuration for electrode oscillator charge/discharge. 8 values available from 500 nA to 64 uA
  * 		PS:			GENCS configuration for clock divisor value. 8 values available from 1 to 128
  * 		NSCN:		GENCS configuration for electrode oscillator count used in making scan. Value can be any value between 1 and 32, inclusive
  * 		TSIEN:		GENCS configuration for enabling/disabling TSI module. 0 to disable, 1 to enable
  * 		EOSF:		GENCS configuration for end-of-scan flag. 0 means scan incomplete, 1 means scan complete. To clear this flag, write 1 to it
  */
 void init_onboard_touch_sensor(void){
	/**
	 * Enable clock to TSI module
	 */
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;

	/**
	 * Configure TSI0 as:
	 * 	- Operate in non-noise mode
	 * 	- Reference oscillator charge and discharge value of 500 nA
	 * 	- Oscillator voltage rails set to default
	 * 	- Electrode oscillator charge and discharge value of 500 nA
	 * 	- Frequency clock divided by 1
	 * 	- Scan electrode 32 times
	 * 	- Enable the TSI module
	 * 	- Write 1 to clear the end of scan flag
	 */
	TSI0->GENCS = \
			TSI_GENCS_MODE(TSI0_MODE_NON_NOISE) |\
			TSI_GENCS_REFCHRG(TSI0_REFCHRG_500_NANO_AMP) |\
			TSI_GENCS_DVOLT(TSI0_DVOLT_DEFAULT) |\
			TSI_GENCS_EXTCHRG(TSI0_EXTCHRG_500_NANO_AMP) |\
			TSI_GENCS_PS(TSI0_PS_CLK_FREQ_DIV_BY_1) |\
			TSI_GENCS_NSCN(TSI0_NSCN_32_TIMES) |\
			TSI_GENCS_TSIEN_MASK |
			TSI_GENCS_EOSF_MASK;

 }

 /**
  * \fn void blink_sequence
  * \brief Blink LED in specific sequence. Color of LED in sequence will be determined by user's touch on the capacitive touch slider. Referenced operations from https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_2/Source/main.c
  * \param N/A
  * \return N/A
  *
  * \detail The LED blink sequence will be:
  * 	- ON for 500 msec, OFF for 500 msec
  * 	- ON for 1000 msec, OFF for 500 msec
  * 	- ON for 2000 msec, OFF for 500 msec
  * 	- ON for 3000 msec, OFF for 500 msec
  * 	- Repeat
  */
 void blink_sequence(void){
	/**
	 *  Force the counter to be placed into memory
	 */
	volatile static int i;

	/**
	 *  To keep track of scanned value from TSI (after subtracting TOUCH_OFFSET)
	 */
	static unsigned int scanned_value;

	/**
	 *
	 */
	color onboard_led;
	color onboard_led_prev;

	/**
	 * Initially the LED will be white
	 */
	onboard_led = white;
	onboard_led_prev = white;
	LED_ON(onboard_led);
	DELAY_500_MSEC();
	LED_OFF(onboard_led);
	DELAY_500_MSEC();

	LED_ON(onboard_led);
	DELAY_1_SEC();
	LED_OFF(onboard_led);
	DELAY_500_MSEC();

	LED_ON(onboard_led);
	DELAY_1_SEC();
	DELAY_1_SEC();
	LED_OFF(onboard_led);
	DELAY_500_MSEC();

	LED_ON(onboard_led);
	DELAY_1_SEC();
	DELAY_1_SEC();
	DELAY_1_SEC();
	LED_OFF(onboard_led);
	DELAY_500_MSEC();

	while(1){
		/**
		 * ON for 500 msec, OFF for 500 msec
		 * ON for 1000 msec, OFF for 500 msec
		 * ON for 2000 msec, OFF for 500 msec
		 * ON for 3000 msec, OFF for 500 msec
		 *
		 */
		GET_TOUCH();
		GET_LED_COLOR();

		LED_ON(onboard_led);
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		LED_OFF(onboard_led);
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();

		LED_ON(onboard_led);
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		LED_OFF(onboard_led);
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();

		LED_ON(onboard_led);
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		LED_OFF(onboard_led);
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();

		LED_ON(onboard_led);
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		LED_OFF(onboard_led);
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
		DELAY_100_MSEC();
		GET_TOUCH();
		GET_LED_COLOR();
	}
 }

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\r\n");

    /**
     * Initialize all 3 on-board LEDs (red, green, blue)
     */
    init_onboard_leds();

    /**
     * Initialize on-board TSI
     */
    init_onboard_touch_sensor();

    /**
     *  Enter blink_sequence which contains an infinite loop
     */
    blink_sequence();

    return 0 ;
}
