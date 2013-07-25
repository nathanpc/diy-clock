/**
 *  main.c
 *  Bed-side clock firmware.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <stdio.h>
#include <string.h>
#include <msp430g2553.h>

// Devices.
#include "PCD8544.h"
#include "adc.h"
#include "buzzer.h"
#include "rtc.h"

// Helpers.
#include "boolean.h"
#include "delay.h"
#include "fonts.h"
#include "ftoa.h"
#include "strings.h"

// Pins.
#define LOWBATT BIT5
#define MOTION  BIT4

// Properties.
#define APROX_VIN 3.6
#define AVG_READS 20

float get_temperature();

void print_date(unsigned int day, unsigned int month);
void print_temp(float temp);
void print_time(unsigned int hour, unsigned int minute, bool pm);
void print_digit(unsigned int pos, unsigned int digit);
void print_symbol(unsigned int pos, unsigned int symbol);
void print_batt_symbol(unsigned int symbol);


/**
 *  The mains.
 */
void main() {
	WDTCTL = WDTPW + WDTHOLD;  // Disable WDT.
	BCSCTL1 = CALBC1_8MHZ;     // 1MHz clock.
	DCOCTL = CALDCO_8MHZ;
	BCSCTL2 &= ~(DIVS_3);      // SMCLK = DCO = 1MHz.

	// Setup for the 32kHz crystal.
	BCSCTL1 |= DIVA_3;  // ACLK / 8.
	BCSCTL3 |= XCAP_3;  // 12.5pF capacitor setting for 32768Hz crystal.

	// Setup the low battery pin.
	P1DIR &= ~(LOWBATT + MOTION);

	// RTC stuff.
	rtc_setup();

	// Setup ADC.
	adc_setup();

	// Setup the LCD stuff.
	lcd_setup();
	delay_ms(1);  // Just to make sure the LCD is ready
	lcd_init();
	lcd_clear();
	print_symbol(':', 0);  // This should only be printed one time, since it never changes.

	// TODO: Setup the buzzer.

	// Enable the interrupts.
	__enable_interrupt();
	// TODO: Test that LMP + GIE thing?

	// TODO: Remove this crap after everything is in the interrupt.
	while (TRUE) {
		/*
		char str[12];
		fetch_adc_readings();
		snprintf(str, sizeof(str), "%d", get_adc_sample(0));
		lcd_set_pos(0,1);
		lcd_print(str);
		lcd_putc(' ');
		*/
	}
}

/**
 *  Gets a average temperature.
 *
 *  @return A temperature.
 */
float get_temperature() {
	float readings[AVG_READS];
	float avg_read = 0.0;

	// Get the values.
	for (unsigned int i = 0; i < AVG_READS; i++) {
		fetch_adc_readings();
		readings[i] = ((get_adc_sample(1) * APROX_VIN) / 1024) * 100;
	}

	// Sum everything.
	for (unsigned int read = 0; read < AVG_READS; read++) {
		avg_read += readings[read];
	}

	// Average the readings.
	return avg_read / AVG_READS;
}

/**
 *  Prints a date on the screen.
 *
 *  @param day A day.
 *  @param month A month.
 */
void print_date(unsigned int day, unsigned int month) {
	// Generate the string.
	char date_str[10];
	snprintf(date_str, 9, "%d %s", day, month_name[month]);

	// Print it.
	lcd_set_pos(0, 0);
	lcd_print(date_str);
}

/**
 *  Prints the temperature on the screen.
 *
 *  @param temp The integer temperature that will be converted to float.
 */
void print_temp(float temp) {
	// Convert float to string.
	char tmp_str[6];
	ftoa(tmp_str, temp, 2);

	// Correct the display if there's only 4 chars to print.
	if (strlen(tmp_str) == 4) {
		lcd_set_pos(43, 0);
		lcd_putc(' ');
	}

	// Position and print.
	lcd_set_pos(68 - (strlen(tmp_str) * 5), 0);
	lcd_print(tmp_str);
	lcd_putc(0x7F);
	lcd_putc('C');
}

/**
 *  Prints the time on the screen.
 *
 *  @param hour The hour.
 *  @param minute The minutes.
 *  @param pm Is is afternoon already?
 */
void print_time(unsigned int hour, unsigned int minute, bool pm) {
	// Updates the minutes.
	if (minute < 10) {
		// Between 0 and 9.
		print_digit(2, 0);
		print_digit(3, minute);
	} else if ((minute >= 10) && (minute < 20)) {
		// Between 10 and 19.
		print_digit(2, 1);
		print_digit(3, minute - 10);
	} else if ((minute >= 20) && (minute < 30)) {
		// Between 20 and 29.
		print_digit(2, 2);
		print_digit(3, minute - 20);
	} else if ((minute >= 30) && (minute < 40)) {
		// Between 30 and 39.
		print_digit(2, 3);
		print_digit(3, minute - 30);
	} else if ((minute >= 40) && (minute < 50)) {
		// Between 40 and 49.
		print_digit(2, 4);
		print_digit(3, minute - 40);
	} else if ((minute >= 50) && (minute < 60)) {
		// Between 50 and 59.
		print_digit(2, 5);
		print_digit(3, minute - 50);
	} else {
		print_digit(2, 8);
		print_digit(3, 8);
	}

	// Updates the hours.
	if (hour < 10) {
		// Between 0 and 9.
		print_digit(0, 0);
		print_digit(1, hour);
	} else if ((hour >= 10) && (hour < 20)) {
		// Between 10 and 19.
		print_digit(0, 1);
		print_digit(1, hour - 10);
	} else if ((hour >= 20) && (hour < 24)) {
		// Between 20 and 23.
		print_digit(0, 2);
		print_digit(1, hour - 20);
	} else {
		// How did we get here?
		print_digit(0, 8);
		print_digit(1, 8);
	}

	// AM/PM
	lcd_set_pos(71, 5);
	if (pm) {
		lcd_print("PM");
	} else {
		lcd_print("AM");
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

/**
 *  Print battery symbol.
 *
 *  @param symbol The symbol number.
 */
void print_batt_symbol(unsigned int symbol) {
	lcd_set_pos(0, 5);

	for (unsigned int col = 0; col < 13; col++) {
		lcd_command(0, batt_sym[symbol][col]);
	}
}

/**
 *  TimerA1 interrupt service routine.
 */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_ISR() {
	rtc_count();

	// Updates the temperature.
	print_temp(get_temperature());

	// TODO: Make some if clauses to only update the things that changed.

	// Update the date.
	print_date(_day, _month);

	// Update the time.
	print_time(_hour, _minute, FALSE);

	// Checks if the batteries are low.
	if ((P1IN & LOWBATT) == 0) {
		print_batt_symbol(0);
	}

/*
	if ((P1IN & MOTION) == 0) {
		lcd_set_pos(0,0);
		lcd_print("Motion!");

		delay_ms(1000);
		lcd_set_pos(0,0);
		lcd_print("        ");
	}
	 */
}
