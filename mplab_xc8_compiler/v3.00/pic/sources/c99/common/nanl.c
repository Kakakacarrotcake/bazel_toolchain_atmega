/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef nanl

long double nanl(const char *x)
{
    return (long double)nanf(x);
}
