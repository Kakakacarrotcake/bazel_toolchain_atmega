/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef atan2l

long double atan2l(long double x, long double y)
{
    return (long double)atan2f((float)x, (float)y);
}
