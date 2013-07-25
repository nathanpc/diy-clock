/**
 *  rtc.h
 *  The clock shit.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#ifndef RTC_H_
#define RTC_H_

// Timing variables.
extern unsigned int _minute;
extern unsigned int _hour;
extern unsigned int _day;
extern unsigned int _month;
extern unsigned int _year;

void rtc_setup();
void rtc_count();

#endif
