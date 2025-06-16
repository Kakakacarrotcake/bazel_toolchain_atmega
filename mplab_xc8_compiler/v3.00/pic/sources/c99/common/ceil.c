/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef ceil

double ceil(double x)
{
    return (double)ceilf((float)x);
}
