/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef atan

double atan(double x)
{
    return (double)atanf((float)x);
}
