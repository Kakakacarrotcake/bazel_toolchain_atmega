/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef sin

double sin(double x)
{
    return (double)sinf((float)x);
}
