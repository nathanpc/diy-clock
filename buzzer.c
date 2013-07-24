/**
 *  buzzer.c
 *  Makes sounds.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include "buzzer.h"
#include <msp430g2553.h>

// Helpers.
#include "boolean.h"
#include "delay.h"
#include "bitio.h"

#define BUZZER BIT2

/**
 *  Setup the buzzer pwm pin.
 */
void buzzer_setup() {
	P1DIR |= BUZZER;  // P1.2 output.
	P1SEL |= BUZZER;  // Set P1.2 to TA1.1

	TA0CCR0  = 100 - 1;   // PWM period.
	TA0CCTL1 = OUTMOD_7;  // CCR1 Reset/Set.
	TA0CCR1  = 0;         // CCR1 PWM duty cycle.
	TA0CTL   = TASSEL_2 + MC_1;
}

/**
 *  Plays a little tune.
 *
 *  @param tone The tone index.
 */
void buzzer_play(const unsigned int tone) {
	//tones[tone];  - Put the tones in a array.
	TA0CCR1 = 50;
}
