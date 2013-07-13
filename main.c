/**
 *  main.c
 *  Bed-side clock firmware.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <stdio.h>
#include <msp430g2553.h>
#include "PCD8544.h"

#include "boolean.h"
#include "delay.h"

void print_digit(unsigned int digit);

// TODO: Move this crazyness to it's own header file.
char big_font[][3][18] = {
	{  // 0
		{ 0b00000000, 0b00000000, 0b11111100, 0b11111100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b11111100, 0b11111100, 0b00000000, 0b00000000 },
		{ 0b00000000, 0b00000000, 0b11111100, 0b11111100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b11111100, 0b11111100, 0b00000000, 0b00000000 },
		{ 0b00000000, 0b00000000, 0b11111100, 0b11111100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b11111100, 0b11111100, 0b00000000, 0b00000000 }
	}
};

/**
 *  The mains.
 */
void main() {
	WDTCTL = WDTPW + WDTHOLD;  // Disable WDT.
	BCSCTL1 = CALBC1_1MHZ;     // 1MHz clock.
	DCOCTL = CALDCO_1MHZ;
	BCSCTL2 &= ~(DIVS_3);      // SMCLK = DCO = 1MHz.

	// Setup the LCD stuff.
	lcd_setup();
	delay_ms(1);  // Just to make sure the LCD is ready
	lcd_init();
	lcd_clear();

	print_digit(0);

	while (TRUE) {
	}
}

/**
 *  Prints a big 18x24 digit in the screen.
 *
 *  @param digit A number from 0 to 9.
 */
void print_digit(unsigned int digit) {
	for (unsigned int row = 0; row < 3; row++) {
		lcd_set_pos(0, row + 2);

		for (unsigned int col = 0; col < 18; col++) {
			lcd_command(0, big_font[digit][row][col]);
		}
	}
}
