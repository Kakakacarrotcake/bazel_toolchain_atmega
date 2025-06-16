/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef lgamma

double lgamma(double x)
{
    return (double)lgammaf((float)x);
}
