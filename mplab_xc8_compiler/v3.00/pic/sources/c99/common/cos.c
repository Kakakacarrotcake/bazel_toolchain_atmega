/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef cos

double cos(double x)
{
    return (double)cosf((float)x);
}
