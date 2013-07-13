/**
 *  main.c
 *  Bed-side clock firmware.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <stdio.h>
#include <msp430g2553.h>

// Devices.
#include "PCD8544.h"

// Helpers.
#include "boolean.h"
#include "delay.h"
#include "fonts.h"

void print_digit(unsigned int pos, unsigned int digit);
void print_symbol(unsigned int pos, unsigned int symbol);

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

	print_digit(0, 8);
	print_digit(1, 8);
	print_digit(2, 8);
	print_digit(3, 8);
	print_symbol(':', 0);  // This should only be printed one time, since it never changes.

	while (TRUE) {
	}
}

/**
 *  Prints a big 18x24 digit in the screen.
 *
 *  @param pos The digit position.
 *  @param digit A number from 0 to 9.
 */
void print_digit(unsigned int pos, unsigned int digit) {
	for (unsigned int row = 0; row < 3; row++) {
		switch (pos) {
		case 0:
			lcd_set_pos(0, row + 2);
			break;
		case 1:
			lcd_set_pos(18, row + 2);
			break;
		case 2:
			lcd_set_pos(48, row + 2);
			break;
		case 3:
			lcd_set_pos(66, row + 2);
			break;
		}

		for (unsigned int col = 0; col < 18; col++) {
			lcd_command(0, big_font[digit][row][col]);
		}
	}
}

/**
 *  Prints a big 12x24 symbol in the screen.
 *
 *  @param pos The symbol position.
 *  @param symbol The symbol number.
 */
void print_symbol(unsigned int pos, unsigned int symbol) {
	for (unsigned int row = 0; row < 3; row++) {
		switch (pos) {
		case ':':
			lcd_set_pos(36, row + 2);
			break;
		}

		for (unsigned int col = 0; col < 12; col++) {
			lcd_command(0, big_font_sym[symbol][row][col]);
		}
	}
}
