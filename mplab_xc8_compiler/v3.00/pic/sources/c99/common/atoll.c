#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

long long atoll(const char *s)
{
	long long n = 0;
	bool neg = false;
	while (isspace(*s)) {
		s++;
	}
	if (*s == '-') {
		neg = true;
		s++;
	}
	else if (*s == '+') {
		s++;
	}
	/* Compute n as a negative number to avoid overflow on LLONG_MIN */
	while (isdigit(*s)) {
		n = 10*n - (int)(*s++ - '0');
	}
	return neg ? n : -n;
}
