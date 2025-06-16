/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef lrint

long lrint(double x)
{
    return lrintf((float)x);
}
