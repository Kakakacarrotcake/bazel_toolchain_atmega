#include <stdlib.h>

div_t div(int num, int den)
{
	div_t retv;
	retv.quot = num / den;
	retv.rem = num % den;
	return retv;
}
