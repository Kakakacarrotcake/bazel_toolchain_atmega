/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef atan2

double atan2(double x, double y)
{
    return (double)atan2f((float)x, (float)y);
}
