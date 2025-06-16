/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef nexttoward

double nexttoward(double x, long double y)
{
    return (double)nexttowardf((float)x, (float)y);
}
