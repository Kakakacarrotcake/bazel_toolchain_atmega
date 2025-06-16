#include	<ctype.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<limits.h>

unsigned long 
__strtoxl(const char * s, char ** endptr, int base, char is_signed)
{
	unsigned long			a, prev_a, limit;
	char			c;
	enum {
		SIGNED = 1,
		SKIP = 2,
		CONV_DONE = 4,
	} flags;
	const char *	cp;

	flags = 0;
	a = 0;
	cp = s;

	if (base > 36 || base == 1) {
		errno = EINVAL;
		goto exit_strtox;
	}

	// skip initial sequence
	if (cp == NULL || *cp == '\0') {
		goto exit_strtox;
	}
	c = *cp;
	while (isspace(c)) {
		c = *(++cp);
	}
	// process subject sequence, if any
	if(c == '-') {
		flags = SIGNED;
		c = *(++cp);
	} else if(c == '+')
		c = *(++cp);
	if(c == '0') {
		c = *(++cp);
		if((base == 0 || base == 16) && (c == 'x' || c == 'X')) {
			base = 16;
			c = *(++cp);
			/* read '0x' so far and we should limit to '0' if the
			 * rest of the input string is not hex*/
			if((isalpha(c) && (((char)((unsigned int)toupper(c)-'A' + 10) >= base))) || !isalnum(c)) {
				flags |= CONV_DONE;
				cp--;
				goto exit_strtox;
			}
		} else {
			flags |= CONV_DONE;
			if(base == 0) {
				base = 8;
			}
		}
	}
	if(base == 0)
		base = 10;
	if (is_signed) {
		limit = (unsigned long)((flags & SIGNED) ? LONG_MIN : LONG_MAX);
	}
	else {
		limit = ULONG_MAX;
	}
	for(;;) {
		prev_a = a;
		if(isalpha(c))
			c = (char)((unsigned int)toupper(c)-'A' + 10);
		else if(isdigit(c))
			c -= '0';
		else
			break;
		if(c >= base)
			break;
		if (!(flags & SKIP)) {
			a = a*(unsigned long)base + c;
			if (prev_a > a || (is_signed && a > limit)) {
				a = limit;
				errno = ERANGE;
				flags |= SKIP;
			}
		}
		c = *(++cp);
		flags |= CONV_DONE;
	}
	if((flags & (SKIP|SIGNED)) == SIGNED) {
		a = -a;
	}
exit_strtox:
	if(endptr) {
		if (!(flags & CONV_DONE)) {
			cp = s;
		}
		*endptr = (char *)(cp);
	}
	return a;
}
