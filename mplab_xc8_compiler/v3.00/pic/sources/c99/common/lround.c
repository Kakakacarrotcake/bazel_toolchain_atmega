/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef lround

long lround(double x)
{
    return lroundf((float)x);
}
