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
	TA1CCR0 = 512;    // 512 -> 1 sec, 30720 -> 1 min.
	TA1CTL = TASSEL_1 + ID_3 + MC_1;  // ACLK, /8, Up mode.
}

// Timer A0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_ISR() {
	sec++;
	lcd_set_pos(0, 2);
	char buffer[100];
	sprintf(buffer, "%d", sec);
	lcd_print(buffer);
}
