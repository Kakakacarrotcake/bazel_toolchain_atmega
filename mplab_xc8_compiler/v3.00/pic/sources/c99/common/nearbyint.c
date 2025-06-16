/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef nearbyint

double nearbyint(double x)
{
    return (double)nearbyintf((float)x);
}
