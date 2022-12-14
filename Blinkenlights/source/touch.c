/**
 * \file    touch.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	09/28/2022
 * \brief   Function definitions for the on-board touch sensor
 */

#include "board.h"
#include "touch.h"

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
			TSI_GENCS_MODE(GENCS_MODE) |\
			TSI_GENCS_REFCHRG(GENCS_REFCHRG) |\
			TSI_GENCS_DVOLT(GENCS_DVOLT) |\
			TSI_GENCS_EXTCHRG(GENCS_EXTCHRG) |\
			TSI_GENCS_PS(GENCS_PS) |\
			TSI_GENCS_NSCN(GENCS_NSCN) |\
			TSI_GENCS_TSIEN_MASK |
			TSI_GENCS_EOSF_MASK;
 }
