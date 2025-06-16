/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef sinh

double sinh(double x)
{
    return (double)sinhf((float)x);
}
