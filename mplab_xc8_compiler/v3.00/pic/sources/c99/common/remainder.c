/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef remainder

double remainder(double x, double y)
{
    return (double)remainderf((float)x, (float)y);
}
