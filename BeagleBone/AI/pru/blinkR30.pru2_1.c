////////////////////////////////////////
//	blinkR30.c
//	Blinks LEDs wired to pr2_pru1 pins by writing register R30 on the PRU
//	Wiring:	P9_30 connects to the plus lead of an LED.  The negative lead of the
//			LED goes to a 220 Ohm resistor.  The other lead of the resistor goes
//			to ground.
//	Setup:	Load the BBAI-PRUOUT_PRU2_1.dtbo overlay
//	See:	1) cloud9/common/prugpio.h for which pins attach to pr2_pru1.
// 			2) End of this file for loading the correct overlay.
//	PRU:	pru2_1
////////////////////////////////////////
#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

volatile register unsigned int __R30;
volatile register unsigned int __R31;

void main(void) {
	// Select which pins to toggle.
	uint32_t gpio = P9_30;

	while(1) {
		__R30 |= gpio;					// Set the GPIO pin to 1
		__delay_cycles(500000000/5);    // Wait 1/2 second
		__R30 &= ~gpio;					// Clear the GPIO pin
		__delay_cycles(500000000/5);	// Wait 1/2 second
		}
	__halt();
}

// You need to load the BBAI-PRUOUT_PRU2_1.dtbo overlay to use this example.
// BBAI-PRUOUT_PRU2_1.dtbo sets pinmux for all PRU2_1 pins to PRUOUT!
// Edit uEnv.txt by executing, $ sudo nano /boot/uEnv.txt
// Add the content within /* */ to uEnv.txt

/* 

enable_uboot_overlays=1
uboot_overlay_addr0=BBAI-PRUOUT_PRU2_1.dtbo

*/

// There are 8 slots for overlays, 
// If addr0 is accupied you can use addr1 - addr7
