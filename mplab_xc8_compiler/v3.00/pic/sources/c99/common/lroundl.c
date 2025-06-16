/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef lroundl

long lroundl(long double x)
{
    return lroundf((float)x);
}
