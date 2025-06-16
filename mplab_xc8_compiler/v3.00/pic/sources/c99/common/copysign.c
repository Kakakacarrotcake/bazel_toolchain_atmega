/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef copysign

double copysign(double x, double y)
{
    return (double)copysignf((float)x, (float)y);
}
