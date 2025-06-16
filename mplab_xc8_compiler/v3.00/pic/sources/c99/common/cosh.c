/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef cosh

double cosh(double x)
{
    return (double)coshf((float)x);
}
