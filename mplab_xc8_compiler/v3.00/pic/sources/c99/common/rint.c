/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef rint

double rint(double x)
{
    return (double)rintf((float)x);
}
