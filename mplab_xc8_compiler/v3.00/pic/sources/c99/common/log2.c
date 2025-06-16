/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef log2

double log2(double x)
{
    return (double)log2f((float)x);
}
