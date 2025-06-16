/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef ilogbl

int ilogbl(long double x)
{
    return ilogbf((float)x);
}
