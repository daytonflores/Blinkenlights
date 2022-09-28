/**
 * \file    led.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	09/28/2022
 * \brief   Macros and function headers for the on-board LED
 */

#ifndef LED_H_
#define LED_H_

/**
 * \typedef color_t
 * Used to define enum of colors, where the colors are the possible values of on-board LED
 */
typedef enum {
	white,
	red,
	green,
	blue
} color_t;

/**
 * \def MASK(x)
 * \param x The amount of bits 1 should be shifted to the left
 *  Referenced from Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers: Embedded Systems
 */
#define MASK(x)\
	((uint32_t)(1UL << (x)))

/**
 * \def PCR_MUX_SEL
 * PCR is a 32-bit register where bits 8:10 are a MUX field
 * 		000: Pin disabled (analog)
 * 		001: GPIO
 * 		010: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		011: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		100: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		101: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		110: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		111: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers: Embedded Systems)
 */
#define PCR_MUX_SEL\
	(1)

/**
 * \def PORTB_RED_LED_PIN
 * On-board red LED is located at PB18
 */
#define PORTB_RED_LED_PIN\
	(18)

/**
 * \def PORTB_GRN_LED_PIN
 * On-board green LED is located at PB19
 */
#define PORTB_GRN_LED_PIN\
	(19)

/**
 * \def PORTB_GRN_LED_PIN
 * On-board blue LED is located at PD1
 */
#define PORTD_BLU_LED_PIN\
	(1)

/**
 * \def LED_ON(x)
 * \param x The color (see color_t definition for available colors) the on-board LED should turn on
 * Turn on on-board LED of specific color
 */
#define LED_ON(x)\
	do{\
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

/**
 * \def LED_OFF(x)
 * \param x The color (see color_t definition for available colors) the on-board LED should turn off
 * Turn off on-board LED of specific color
 */
#define LED_OFF(x)\
	do{\
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

/**
 * \def LED_TOGGLE(x)
 * \param x The color (see color_t definition for available colors) the on-board LED should toggle
 * Toggle on-board LED of specific color
 */
#define LED_TOGGLE(x)\
	do{\
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

/**
 * \def INIT_LED_COLOR
 *  The color that the on-board LED should be during initial blink sequence, before entering the infinite loop
 */
#define INIT_LED_COLOR\
	(white)
/**
 * \def GET_LED_COLOR()
 * Based on the scanned value from TSI module, calculate and store the color that the on-board LED should display
 */
#define GET_LED_COLOR()\
	do{\
		onboard_led_prev = onboard_led;\
		\
		if(scanned_value < TOUCH_UNTOUCHED_MAX){\
			onboard_led = onboard_led_prev;\
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
	}while(0)

/**
 * \def PRINTF_LED_COLOR_CHANGE(x)
 * \param The new color LED is being changed to
 * Print the new color that on-board LED is being changed to
 */
#define PRINTF_LED_COLOR_CHANGE(x)\
	(PRINTF("CHANGE LED TO %s\r\n", (x == red) ? "RED" : ((x == green) ? "GREEN" : ((x == blue) ? "BLUE" : "?"))));

/**
 * \fn void init_onboard_leds
 * \brief Initialize all 3 on-board LEDs as GPIO outputs and turn them all off. Referenced operations from https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_2/Source/main.c
 * \param N/A
 * \return N/A
 *
 *  Many operations were referenced from Alexander G Dean (Chapter 2 of Embedded Systems Fundamentals with ARM Cortex-M Based Microcontrollers)
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
void init_onboard_leds(void);

/**
 * \fn void init_blink_sequence
 * \brief Blink LED in specific sequence. Color of LED will be white
 * \param N/A
 * \return N/A
 *
 *  The LED blink sequence will be:
 * 		- ON for 500 msec, OFF for 500 msec
 * 		- ON for 1000 msec, OFF for 500 msec
 * 		- ON for 2000 msec, OFF for 500 msec
 * 		- ON for 3000 msec, OFF for 500 msec
 * 		- Repeat
 */
void init_blink_sequence(void);

/**
 * \fn void blink_sequence
 * \brief Blink LED in specific sequence. Color of LED in sequence will be determined by user's touch on the capacitive touch slider. Referenced operations from https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_2/Source/main.c
 * \param N/A
 * \return N/A
 *
 *  The LED blink sequence will be:
 * 		- ON for 500 msec, OFF for 500 msec
 * 		- ON for 1000 msec, OFF for 500 msec
 * 		- ON for 2000 msec, OFF for 500 msec
 * 		- ON for 3000 msec, OFF for 500 msec
 * 		- Repeat
 */
void blink_sequence(void);

#endif /* LED_H_ */
