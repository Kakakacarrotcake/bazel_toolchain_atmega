/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef log1p

double log1p(double x)
{
    return (double)log1pf((float)x);
}
