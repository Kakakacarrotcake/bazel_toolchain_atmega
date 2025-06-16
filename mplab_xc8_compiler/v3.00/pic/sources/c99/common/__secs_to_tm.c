#include "../time_impl.h"

/* 2000-03-01 (mod 400 year, immediately after feb29 */
#define LEAPOCH (946684800 + 86400*(31+29))

#define DAYS_PER_400Y (365*400L + 97)
#define DAYS_PER_100Y (365*100L + 24)
#define DAYS_PER_4Y   (365*4L   + 1)

int __secs_to_tm(unsigned long t, struct tm *tm)
{
	long days, years;
	long remdays, remsecs, remyears;
	long qc_cycles, c_cycles, q_cycles;
	int months;
	int wday, yday, leap;
	static const char days_in_month[] = {31,30,31,30,31,31,30,31,30,31,31,29};

	unsigned long diffsecs = t - ((unsigned long)LEAPOCH);
	/* If t is greater than LEAPOCH, then t - LEAPOCH can overflow
       signed long, but not unsigned long. So compute difference using
       unsigned arithmetic. The quotient and remainder after dividing
       by 86400 will fit in a signed long. */
    if (t > LEAPOCH) {
        days = diffsecs / 86400;
		remsecs = diffsecs % 86400;
    } else {
	   days = (long)diffsecs / 86400;
	   remsecs = (long)diffsecs % 86400;
    }

	if (remsecs < 0) {
		remsecs += 86400;
		days--;
	}

	wday = (3+days)%7;
	if (wday < 0) wday += 7;

	qc_cycles = (long)(days / DAYS_PER_400Y);
	remdays = days % DAYS_PER_400Y;
	if (remdays < 0) {
		remdays += DAYS_PER_400Y;
		qc_cycles--;
	}

	c_cycles = remdays / DAYS_PER_100Y;
	if (c_cycles == 4) c_cycles--;
	remdays -= c_cycles * DAYS_PER_100Y;

	q_cycles = remdays / DAYS_PER_4Y;
	if (q_cycles == 25) q_cycles--;
	remdays -= q_cycles * DAYS_PER_4Y;

	remyears = remdays / 365;
	if (remyears == 4) remyears--;
	remdays -= remyears * 365;

	leap = !remyears && (q_cycles || !c_cycles);
	yday = (int)(remdays + 31 + 28 + leap);
	if (yday >= 365+leap) yday -= 365+leap;

	years = remyears + 4*q_cycles + 100*c_cycles + 400L*qc_cycles;

	for (months=0; days_in_month[months] <= remdays; months++)
		remdays -= days_in_month[months];

	if (months >= 10) {
		months -= 12;
		years++;
	}

	tm->tm_year = (int)(years + 100);
	tm->tm_mon = months + 2;
	tm->tm_mday = (int)(remdays + 1);
	tm->tm_wday = wday;
	tm->tm_yday = yday;

	tm->tm_hour = (int)(remsecs / 3600);
	tm->tm_min = remsecs / 60 % 60;
	tm->tm_sec = remsecs % 60;

	return 0;
}
