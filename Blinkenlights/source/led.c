/**
 * \file    led.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	09/28/2022
 * \brief   Function definitions for the on-board LED
 */

#ifdef DEBUG
#include "fsl_debug_console.h"
#include "board.h"
#include "MKL25Z4.h"
#include "led.h"
#include "touch.h"
#include "delay.h"
#endif

#ifdef NDEBUG
#include "board.h"
#include "MKL25Z4.h"
#include "led.h"
#include "touch.h"
#include "delay.h"
#endif

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
	PORTB->PCR[PORTB_RED_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_SEL);
	PORTB->PCR[PORTB_GRN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PORTB_GRN_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_SEL);
	PORTD->PCR[PORTD_BLU_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PORTD_BLU_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_SEL);

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
#ifdef DEBUG
	PRINTF("START TIMER 500\r\n");
    LED_ON(red);
	for(i = 0; i < 5; i++){
		DELAY_100_MSEC();
	}

	PRINTF("START TIMER 100\r\n");
	LED_OFF(red);
	DELAY_100_MSEC();

	PRINTF("START TIMER 500\r\n");
    LED_ON(green);
	for(i = 0; i < 5; i++){
		DELAY_100_MSEC();
	}

	PRINTF("START TIMER 100\r\n");
    LED_OFF(green);
	DELAY_100_MSEC();

	PRINTF("START TIMER 500\r\n");
    LED_ON(blue);
	for(i = 0; i < 5; i++){
		DELAY_100_MSEC();
	}

	PRINTF("START TIMER 100\r\n");
    LED_OFF(blue);
	DELAY_100_MSEC();

	PRINTF("START TIMER 100\r\n");
	LED_ON(white);
	DELAY_100_MSEC();

	PRINTF("START TIMER 100\r\n");
	LED_OFF(white);
	DELAY_100_MSEC();

	PRINTF("START TIMER 100\r\n");
	LED_ON(white);
	DELAY_100_MSEC();

	PRINTF("START TIMER 100\r\n");
	LED_OFF(white);
	DELAY_100_MSEC();
#endif

#ifdef NDEBUG
    LED_ON(red);
	for(i = 0; i < 5; i++){
		DELAY_100_MSEC();
	}

	LED_OFF(red);
	DELAY_100_MSEC();

    LED_ON(green);
	for(i = 0; i < 5; i++){
		DELAY_100_MSEC();
	}

    LED_OFF(green);
	DELAY_100_MSEC();

    LED_ON(blue);
	for(i = 0; i < 5; i++){
		DELAY_100_MSEC();
	}

    LED_OFF(blue);
	DELAY_100_MSEC();

	LED_ON(white);
	DELAY_100_MSEC();

	LED_OFF(white);
	DELAY_100_MSEC();

	LED_ON(white);
	DELAY_100_MSEC();

	LED_OFF(white);
	DELAY_100_MSEC();
#endif
}

void init_blink_sequence(void){

	/**
	 *  Force the counter to be placed into memory
	 */
	volatile static int i;

	/**
	 * ON for 500 msec, OFF for 500 msec
	 * ON for 1000 msec, OFF for 500 msec
	 * ON for 2000 msec, OFF for 500 msec
	 * ON for 3000 msec, OFF for 500 msec
	 *
	 */

#ifdef DEBUG
		PRINTF("START TIMER 500\r\n");
		LED_ON(INIT_LED_COLOR);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
		}

		PRINTF("START TIMER 500\r\n");
		LED_OFF(INIT_LED_COLOR);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
		}

		PRINTF("START TIMER 1000\r\n");
		LED_ON(INIT_LED_COLOR);
		for(i = 0; i < 10; i++){
			DELAY_100_MSEC();
		}

		PRINTF("START TIMER 500\r\n");
		LED_OFF(INIT_LED_COLOR);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
		}

		PRINTF("START TIMER 2000\r\n");
		LED_ON(INIT_LED_COLOR);
		for(i = 0; i < 20; i++){
			DELAY_100_MSEC();
		}

		PRINTF("START TIMER 500\r\n");
		LED_OFF(INIT_LED_COLOR);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
		}

		PRINTF("START TIMER 3000\r\n");
		LED_ON(INIT_LED_COLOR);
		for(i = 0; i < 30; i++){
			DELAY_100_MSEC();
		}

		PRINTF("START TIMER 500\r\n");
		LED_OFF(INIT_LED_COLOR);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
		}
#endif

#ifdef NDEBUG
		LED_ON(INIT_LED_COLOR);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
		}

		LED_OFF(INIT_LED_COLOR);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
		}

		LED_ON(INIT_LED_COLOR);
		for(i = 0; i < 10; i++){
			DELAY_100_MSEC();
		}

		LED_OFF(INIT_LED_COLOR);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
		}

		LED_ON(INIT_LED_COLOR);
		for(i = 0; i < 20; i++){
			DELAY_100_MSEC();
		}

		LED_OFF(INIT_LED_COLOR);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
		}

		LED_ON(INIT_LED_COLOR);
		for(i = 0; i < 30; i++){
			DELAY_100_MSEC();
		}

		LED_OFF(INIT_LED_COLOR);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
		}
#endif
}

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
	color_t onboard_led;
	color_t onboard_led_prev;

	onboard_led = white;
	onboard_led_prev = white;

	/**
	 * ON for 500 msec, OFF for 500 msec
	 * ON for 1000 msec, OFF for 500 msec
	 * ON for 2000 msec, OFF for 500 msec
	 * ON for 3000 msec, OFF for 500 msec
	 *
	 */



#ifdef DEBUG

	GET_TOUCH();
	PRINTF_TOUCH(scanned_value);
	GET_LED_COLOR();
	if(onboard_led != onboard_led_prev){
		PRINTF_LED_COLOR_CHANGE(onboard_led);
	}

	while(1){
		PRINTF("START TIMER 500\r\n");
		LED_ON(onboard_led);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			PRINTF_TOUCH(scanned_value);
			GET_LED_COLOR();
			if(onboard_led != onboard_led_prev){
				PRINTF_LED_COLOR_CHANGE(onboard_led);
			}
		}

		PRINTF("START TIMER 500\r\n");
		LED_OFF(onboard_led);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			PRINTF_TOUCH(scanned_value);
			GET_LED_COLOR();
			if(onboard_led != onboard_led_prev){
				PRINTF_LED_COLOR_CHANGE(onboard_led);
			}
		}

		PRINTF("START TIMER 1000\r\n");
		LED_ON(onboard_led);
		for(i = 0; i < 10; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			PRINTF_TOUCH(scanned_value);
			GET_LED_COLOR();
			if(onboard_led != onboard_led_prev){
				PRINTF_LED_COLOR_CHANGE(onboard_led);
			}
		}

		PRINTF("START TIMER 500\r\n");
		LED_OFF(onboard_led);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			PRINTF_TOUCH(scanned_value);
			GET_LED_COLOR();
			if(onboard_led != onboard_led_prev){
				PRINTF_LED_COLOR_CHANGE(onboard_led);
			}
		}

		PRINTF("START TIMER 2000\r\n");
		LED_ON(onboard_led);
		for(i = 0; i < 20; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			PRINTF_TOUCH(scanned_value);
			GET_LED_COLOR();
			if(onboard_led != onboard_led_prev){
				PRINTF_LED_COLOR_CHANGE(onboard_led);
			}
		}

		PRINTF("START TIMER 500\r\n");
		LED_OFF(onboard_led);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			PRINTF_TOUCH(scanned_value);
			GET_LED_COLOR();
			if(onboard_led != onboard_led_prev){
				PRINTF_LED_COLOR_CHANGE(onboard_led);
			}
		}

		PRINTF("START TIMER 3000\r\n");
		LED_ON(onboard_led);
		for(i = 0; i < 30; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			PRINTF_TOUCH(scanned_value);
			GET_LED_COLOR();
			if(onboard_led != onboard_led_prev){
				PRINTF_LED_COLOR_CHANGE(onboard_led);
			}
		}

		PRINTF("START TIMER 500\r\n");
		LED_OFF(onboard_led);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			PRINTF_TOUCH(scanned_value);
			GET_LED_COLOR();
			if(onboard_led != onboard_led_prev){
				PRINTF_LED_COLOR_CHANGE(onboard_led);
			}
		}
	}
#endif

#ifdef NDEBUG

	GET_TOUCH();
	GET_LED_COLOR();

	while(1){
		LED_ON(onboard_led);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			GET_LED_COLOR();
		}

		LED_OFF(onboard_led);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			GET_LED_COLOR();
		}

		LED_ON(onboard_led);
		for(i = 0; i < 10; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			GET_LED_COLOR();
		}

		LED_OFF(onboard_led);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			GET_LED_COLOR();
		}

		LED_ON(onboard_led);
		for(i = 0; i < 20; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			GET_LED_COLOR();
		}

		LED_OFF(onboard_led);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			GET_LED_COLOR();
		}

		LED_ON(onboard_led);
		for(i = 0; i < 30; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			GET_LED_COLOR();
		}

		LED_OFF(onboard_led);
		for(i = 0; i < 5; i++){
			DELAY_100_MSEC();
			GET_TOUCH();
			GET_LED_COLOR();
		}
	}
#endif
}
