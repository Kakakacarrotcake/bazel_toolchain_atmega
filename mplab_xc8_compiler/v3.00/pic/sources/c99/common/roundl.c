/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef roundl

long double roundl(long double x)
{
    return (long double)roundf((float)x);
}
