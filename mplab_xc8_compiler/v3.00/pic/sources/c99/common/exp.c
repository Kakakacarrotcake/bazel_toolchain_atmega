/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef exp

double exp(double x)
{
    return (double)expf((float)x);
}
