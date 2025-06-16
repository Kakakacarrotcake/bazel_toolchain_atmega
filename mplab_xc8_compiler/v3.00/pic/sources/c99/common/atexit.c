#include <stdlib.h>

/* The C99 standard stipulates a minimum capacity of 32 function registrations,
 * but this is excessive for these targets.
 */ 
#define COUNT 3

static void (*funcs[COUNT])(void);
static signed char count;

void __funcs_on_exit()
{
	void (*func)(void);
	while (count > 0) {
		func = funcs[--count];
		func();
	}
}

int atexit(void (*func)(void))
{
	if (count == COUNT) {
		return -1;
	}
	funcs[count++] = func;
	return 0;
}
