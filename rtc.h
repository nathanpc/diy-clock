/**
 *  rtc.h
 *  The clock shit.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#ifndef RTC_H_
#define RTC_H_

// Timing variables.
unsigned int minute = 0;
unsigned int hour   = 0;
unsigned int day    = 1;
unsigned int month  = 0;
unsigned int year   = 2013;

void rtc_setup();
void rtc_count();

#endif
