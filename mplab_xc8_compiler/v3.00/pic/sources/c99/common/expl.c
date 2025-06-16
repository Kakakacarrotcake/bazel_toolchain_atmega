/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef expl

long double expl(long double x)
{
    return (long double)expf((float)x);
}
