/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef cbrt

double cbrt(double x)
{
    return (double)cbrtf((float)x);
}
