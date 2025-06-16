/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef ldexp

double ldexp(double x, int y)
{
    return (double)ldexpf((float)x, y);
}
