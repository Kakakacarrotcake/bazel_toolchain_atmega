/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef coshl

long double coshl(long double x)
{
    return (long double)coshf((float)x);
}
