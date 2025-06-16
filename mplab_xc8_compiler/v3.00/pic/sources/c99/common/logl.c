/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef logl

long double logl(long double x)
{
    return (long double)logf((float)x);
}
