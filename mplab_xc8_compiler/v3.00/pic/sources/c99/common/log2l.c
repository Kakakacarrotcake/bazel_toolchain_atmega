/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef log2l

long double log2l(long double x)
{
    return (long double)log2f((float)x);
}
