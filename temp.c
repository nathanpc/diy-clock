/**
 *  PCD8544.c
 *  PCD8544 (bit bang) driver for the MSP430G2553 based on RobG's code.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include "temp.h"
#include <msp430g2553.h>

// Helpers.
#include "boolean.h"
#include "delay.h"
#include "bitio.h"

// Pins.
#define TMP BIT6

/**
 *  Setup the pins and the ADC.
 */
void temp_setup() {
	// Turn P1.6 into a ADC.
	P1DIR &= ~(TMP);
	P1SEL |= TMP;

	// Setup the ADC.
	ADC10CTL1 = INCH_6 + ADC10DIV_3;  // Channel 6, ADC10CLK/4
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  // Vcc & Vss as reference.
	ADC10AE0 |= TMP;  // P1.6 ADC option.
}
