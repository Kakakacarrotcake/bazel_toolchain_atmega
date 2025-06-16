/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef log1pl

long double log1pl(long double x)
{
    return (long double)log1pf((float)x);
}
