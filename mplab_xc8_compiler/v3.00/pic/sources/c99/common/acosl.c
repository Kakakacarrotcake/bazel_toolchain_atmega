/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef acosl

long double acosl(long double x)
{
    return (long double)acosf((float)x);
}
