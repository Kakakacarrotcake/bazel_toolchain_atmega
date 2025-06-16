/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef lgammal

long double lgammal(long double x)
{
    return (long double)lgammaf((float)x);
}
