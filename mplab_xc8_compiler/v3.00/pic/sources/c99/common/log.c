/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef log

double log(double x)
{
    return (double)logf((float)x);
}
