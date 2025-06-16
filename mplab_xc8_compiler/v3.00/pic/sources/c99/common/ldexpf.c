#include <math.h>

#ifdef ldexpf
#undef ldexpf
#endif

float ldexpf(float x, int n)
{
	return scalbnf(x, n);
}
