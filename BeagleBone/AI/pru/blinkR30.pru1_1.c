////////////////////////////////////////
//	blinkR30.c
//	Blinks LEDs wired to pr1_pru1 pins by writing register R30 on the PRU
//	Wiring:	P9_16 connects to the plus lead of an LED.  The negative lead of the
//			LED goes to a 220 Ohm resistor.  The other lead of the resistor goes
//			to ground.
//	Setup:	None
//	See:	cloud9/common/prugpio.h to see which pins attach to pr1_pru1
//	PRU:	pru1_1
////////////////////////////////////////
#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

volatile register unsigned int __R30;
volatile register unsigned int __R31;

void main(void) {
	// Select which pins to toggle.
	uint32_t gpio = P9_16;

	while(1) {
		__R30 |= gpio;					// Set the GPIO pin to 1
		__delay_cycles(500000000/5);    // Wait 1/2 second
		__R30 &= ~gpio;					// Clear the GPIO pin
		__delay_cycles(500000000/5);	// Wait 1/2 second
		}
	__halt();
}

// You need to load this overlay to configure pimux of all the pr1_pru1 pins as pruout
// https://github.com/lorforlinux/bb.org-overlays/blob/pru/src/arm/BBAI-PRUOUT_PRU1_1.dts
