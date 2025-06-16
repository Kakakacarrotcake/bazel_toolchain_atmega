/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef acoshl

long double acoshl(long double x)
{
    return (long double)acoshf((float)x);
}
