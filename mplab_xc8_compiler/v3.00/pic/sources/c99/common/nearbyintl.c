/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef nearbyintl

long double nearbyintl(long double x)
{
    return (long double)nearbyintf((float)x);
}
