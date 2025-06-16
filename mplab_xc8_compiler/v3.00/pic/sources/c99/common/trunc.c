/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef trunc

double trunc(double x)
{
    return (double)truncf((float)x);
}
