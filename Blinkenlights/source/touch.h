/**
 * \file    led.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	09/28/2022
 * \brief   Macros and function headers for the on-board touch sensor
 */

#ifndef TOUCH_H_
#define TOUCH_H_

/**
 *
  * 		TSIEN:		GENCS configuration for enabling/disabling TSI module. 0 to disable, 1 to enable
  * 		EOSF:		GENCS configuration for end-of-scan flag. 0 means scan incomplete, 1 means scan complete. To clear this flag, write 1 to it
 */

/**
 * \def GENCS_MODE
 *  Configuration for TSI operation mode. 4 modes available:
 * 		0: Non-noise mode
 * 		1: Noise threshold detection mode
 * 		2: Noise threshold detection mode
 * 		3: Automatic noise detection mode
 */
#define GENCS_MODE\
	(0UL)

/**
 * \def GENCS_REFCHRG
 *  Configuration for reference oscillator charge/discharge. 8 values available:
 *  	0: 500 nA
 *  	1: 1 uA
 *  	2: 2 uA
 *  	3: 4 uA
 *  	4: 8 uA
 *  	5: 16 uA
 *  	6: 32 uA
 *  	7: 64 uA
 */
#define GENCS_REFCHRG\
	(0UL)

/**
 * \def GENCS_DVOLT
 *  Configuration for oscillator voltage rails. 4 levels available
  * 	0:
  * 	1:
  * 	2:
  * 	3:
 */
#define GENCS_DVOLT\
	(0UL)

/**
 * \def GENCS_EXTCHRG
 *  Configuration for electrode oscillator charge/discharge. 8 values available from 500 nA to 64 uA
 *  	0: 500 nA
 *  	1: 1 uA
 *  	2: 2 uA
 *  	3: 4 uA
 *  	4: 8 uA
 *  	5: 16 uA
 *  	6: 32 uA
 *  	7: 64 uA
 */
#define GENCS_EXTCHRG\
	(0UL)

/**
 * \def GENCS_PS
 *  Configuration for clock divisor value. 8 values available:
 *  	0: 1
 *  	1: 2
 *  	2: 4
 *  	3: 8
 *  	4: 16
 *  	5: 32
 *  	6: 64
 *  	7: 128
 */
#define GENCS_PS\
	(0UL)

/**
 * \def GENCS_NSCN
 *  Configuration for electrode oscillator count used in making scan. Value can be any value between 1 and 32, inclusive
 */
#define GENCS_NSCN\
	(31UL)

/**
 * \def TSI0_CHANNEL_10
 * TSI0 channel 10
 */
#define TSI0_CHANNEL_10\
	(10UL)

/**
 * \def TOUCH_OFFSET
 * The amount to offset each scanned value by. Calculated by having 0 offset and then printing the scanned value when touch sensor is not being touched
 */
#define TOUCH_OFFSET\
	(700)

/**
 * \def TOUCH_DATA
 * Grab the 16-bit scanned value data from the TSI0 Data register
 */
#define TOUCH_DATA\
	(TSI0->DATA & 0xFFFF)

/**
 * \def TOUCH_UNTOUCHED_MAX
 * Any scanned_value - TOUCH_OFFSET less than 100 will consider the touch sensor to be untouched
 */
#define TOUCH_UNTOUCHED_MAX\
	(100)

/**
 * \def TOUCH_LEFT_MAX
 * Any scanned_value - TOUCH_OFFSET greater than TOUCH_UNTOUCHED_MAX but less than 500  will consider the touch sensor to be touched on the left side
 */
#define TOUCH_LEFT_MAX\
	(500)

/**
 * \def TOUCH_RIGHT_MIN
 * Any scanned_value - TOUCH_OFFSET greater than TOUCH_LEFT_MAX but less than 2000  will consider the touch sensor to be touched on the right side
 */
#define TOUCH_RIGHT_MIN\
	(2000)


/**
 * \def GET_TOUCH()
 * Start scanning the touch sensor and store the value into scanned_value
 */
#define GET_TOUCH()\
	do{\
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

 /**
  * \def PRINTF_TOUCH(x)
  * \param The scanned_value after subtracting TOUCH_OFFSET to print to console
  * Print the slider value after subtracting offset
  */
#define PRINTF_TOUCH(x)\
	(PRINTF("SLIDER VALUE %d\r\n", x))

 /**
  * \fn void init_onboard_touch_sensor
  * \brief Initialize capacitive touch sensor
  * \param N/A
  * \return N/A
  *
  *  Many operations were referenced from Alexander G Dean's TSI project on GitHub (https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense)
  * 		TSI: 		Touch Sensing Interface refers to interfacing with the on-board capacitive touch pad
  * 		SIM:		System Integration Module is a peripheral containing many control registers, including SCGC5
  * 		SCGC5:		System Clock Gating Control Register 5 is a register containing different controls, including clock gating for TSI
  * 		TSI0:		TSI peripheral 0 (there are 16 channels for each TSI peripheral, though this board only has 1 on-board TSI peripheral)
  * 		GENCS:		General Control and Status Register for TSI module (where each channel 0-15 has its own GENCS register)
  * 		TSIEN:		GENCS configuration for enabling/disabling TSI module. 0 to disable, 1 to enable
  * 		EOSF:		GENCS configuration for end-of-scan flag. 0 means scan incomplete, 1 means scan complete. To clear this flag, write 1 to it
  */
void init_onboard_touch_sensor(void);

#endif /* TOUCH_H_ */
