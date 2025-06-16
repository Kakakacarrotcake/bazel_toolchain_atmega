/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef frexp

double frexp(double x, int *y)
{
    return (double)frexpf((float)x, y);
}
