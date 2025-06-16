/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef sqrt

double sqrt(double x)
{
    return (double)sqrtf((float)x);
}
