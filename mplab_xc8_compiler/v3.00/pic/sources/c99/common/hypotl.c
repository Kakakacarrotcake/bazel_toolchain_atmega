/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef hypotl

long double hypotl(long double x, long double y)
{
    return (long double)hypotf((float)x, (float)y);
}
