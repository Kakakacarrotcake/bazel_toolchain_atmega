/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef powl

long double powl(long double x, long double y)
{
    return (long double)powf((float)x, (float)y);
}
