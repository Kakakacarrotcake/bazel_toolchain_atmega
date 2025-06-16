/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef fdim

double fdim(double x, double y)
{
    return (double)fdimf((float)x, (float)y);
}
