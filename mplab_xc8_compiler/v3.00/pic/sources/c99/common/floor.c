/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef floor

double floor(double x)
{
    return (double)floorf((float)x);
}
