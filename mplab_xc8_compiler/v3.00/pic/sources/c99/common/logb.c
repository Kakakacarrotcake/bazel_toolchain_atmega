/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef logb

double logb(double x)
{
    return (double)logbf((float)x);
}
