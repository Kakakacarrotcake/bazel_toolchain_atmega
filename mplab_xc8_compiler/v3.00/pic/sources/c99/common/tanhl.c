/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef tanhl

long double tanhl(long double x)
{
    return (long double)tanhf((float)x);
}
