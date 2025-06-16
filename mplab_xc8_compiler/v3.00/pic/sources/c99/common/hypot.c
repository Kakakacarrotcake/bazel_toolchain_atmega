/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef hypot

double hypot(double x, double y)
{
    return (double)hypotf((float)x, (float)y);
}
