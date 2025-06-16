/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef log10

double log10(double x)
{
    return (double)log10f((float)x);
}
