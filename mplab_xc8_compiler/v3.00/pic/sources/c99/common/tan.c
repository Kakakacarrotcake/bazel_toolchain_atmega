/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef tan

double tan(double x)
{
    return (double)tanf((float)x);
}
