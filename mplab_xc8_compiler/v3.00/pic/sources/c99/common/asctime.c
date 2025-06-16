/*
 *	asctime - no daylight saving
 */

#include	<time.h>

static void
put2d(char * cp, char  i, char  cl, char  ct)
{
	*cp = ct;
	*--cp = i%10 + '0';
	if(i /= 10)
		*--cp = i + '0';
	else
		*--cp = cl;
}

char *
asctime(const struct tm * tim)
{
	register const char *	s;
	char *			cp;
	static char	buf[27];

	s = &"SunMonTueWedThuFriSat"[tim->tm_wday*3];
	cp = buf;
	*cp++ = *s++;
	*cp++ = *s++;
	*cp++ = *s;
	*cp++ = ' ';
	s = &"JanFebMarAprMayJunJulAugSepOctNovDec"[tim->tm_mon*3];
	*cp++ = *s++;
	*cp++ = *s++;
	*cp = *s;
	buf[7] = ' ';
	put2d(buf+10, (char)tim->tm_mday, ' ', ' ');
	put2d(buf+13, (char)tim->tm_hour, '0', ':');
	put2d(buf+16, (char)tim->tm_min, '0', ':');
	put2d(buf+19, (char)tim->tm_sec, '0', ' ');
	put2d(buf+22, (char)(tim->tm_year/100 + 19), ' ', 0);
	put2d(buf+24, (char)tim->tm_year%100, '0', '\n');
	return(buf);
}

