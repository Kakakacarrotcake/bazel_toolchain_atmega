/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef nan

double nan(const char *x)
{
    return (double)nanf(x);
}
