/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef nexttowardl

long double nexttowardl(long double x, long double y)
{
    return (long double)nexttowardf((float)x, (float)y);
}
