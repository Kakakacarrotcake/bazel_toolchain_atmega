#include	<assert.h>
#include	<stdio.h>
#include	<stdlib.h>

/* The default implementation of __assert_fail */

_Noreturn void 
__assert_fail(const char *expr, const char *file, int line, const char *func)
{
	printf("Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
	abort();
}
