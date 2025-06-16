/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef remquol

long double remquol(long double x, long double y, int *z)
{
    return (long double)remquof((float)x, (float)y, z);
}
