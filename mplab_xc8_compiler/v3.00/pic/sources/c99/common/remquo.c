/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef remquo

double remquo(double x, double y, int *z)
{
    return (double)remquof((float)x, (float)y, z);
}
