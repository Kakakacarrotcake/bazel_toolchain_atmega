/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef asin

double asin(double x)
{
    return (double)asinf((float)x);
}
