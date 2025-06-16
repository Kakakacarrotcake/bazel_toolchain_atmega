/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef ilogb

int ilogb(double x)
{
    return ilogbf((float)x);
}
