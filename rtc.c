/**
 *  rtc.c
 *  The clock shit.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include "rtc.h"
#include <msp430g2553.h>

// Helpers.
#include "boolean.h"
#include "delay.h"
#include "bitio.h"


/**
 *  Setup the timer.
 */
void rtc_setup() {
	TA1CCTL0 = CCIE;  // CCR0 interrupt enabled.
	TA1CCR0 = 30720;  // 512 -> 1 sec, 30720 -> 1 min.
	TA1CTL = TASSEL_1 + ID_3 + MC_1;  // ACLK, /8, Up mode.
}

/**
 *  It counts.
 */
void rtc_count() {
	// Another minute passed.
	minute++;

	// Check if it's time to go to the hours.
	if (minute >= 60) {
		// Resets the minute counter and add one hour.
		minute = 0;
		hour++;
	}

	// Check if a day has passed.
	if (hour >= 24) {
		// Resets the hour counter and add another day.
		hour = 0;
		day++;
	}

	// Check if a month has passed. (> is because it starts at 1)
	if (day > 31) {  // TODO: Get the day from the fucking month and stop assuming shit.
		// Resets the day counter and add another month.
		day = 1;  // Yes, they start at 1, not 0. (sadly)
		month++;
	}

	// Check if a fucking year passed. (I think the batteries should've died by now)
	if (month >= 12) {
		// Resets the month counter and adds a year.
		month = 0;
		year++;
	}

	// TODO: Check for the leap year crap.
}
