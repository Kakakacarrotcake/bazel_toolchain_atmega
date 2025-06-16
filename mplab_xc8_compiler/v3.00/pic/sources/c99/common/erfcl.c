/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef erfcl

long double erfcl(long double x)
{
    return (long double)erfcf((float)x);
}
