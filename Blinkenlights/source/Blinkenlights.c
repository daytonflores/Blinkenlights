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
#define RED_LED_ON()				(PTB->PCOR |= MASK(PORTB_RED_LED_PIN))
#define RED_LED_OFF()				(PTB->PSOR |= MASK(PORTB_RED_LED_PIN))
#define RED_LED_TOGGLE()			(PTB->PTOR |= MASK(PORTB_RED_LED_PIN))
#define GRN_LED_ON()				(PTB->PCOR |= MASK(PORTB_GRN_LED_PIN))
#define GRN_LED_OFF()				(PTB->PSOR |= MASK(PORTB_GRN_LED_PIN))
#define GRN_LED_TOGGLE()			(PTB->PTOR |= MASK(PORTB_GRN_LED_PIN))
#define BLU_LED_ON()				(PTD->PCOR |= MASK(PORTD_BLU_LED_PIN))
#define BLU_LED_OFF()				(PTD->PSOR |= MASK(PORTD_BLU_LED_PIN))
#define BLU_LED_TOGGLE()			(PTD->PTOR |= MASK(PORTD_BLU_LED_PIN))


/**
 * \fn void init_onboard_leds (void)
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
     * Enable clock to Port B for red + green on-board LEDs
     * Enable clock to Port D for blue on-board LED
     *
     * SCGC5
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
    RED_LED_OFF();
    GRN_LED_OFF();
    BLU_LED_OFF();
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

    /* Force the counter to be placed into memory. */
    volatile static int i;

    /**
     * Initialize all 3 on-board LEDs (red, green, blue)
     */
    init_onboard_leds();

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {

    	RED_LED_TOGGLE();
    	GRN_LED_TOGGLE();
    	BLU_LED_TOGGLE();

    	i = 0;

    	while((i++) < 5000000){
            /* 'Dummy' NOP to allow source level single stepping of
                tight while() loop */
            __asm volatile ("nop");
    	}
    }
    return 0 ;
}
