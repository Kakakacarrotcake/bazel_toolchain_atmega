/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef nextafter

double nextafter(double x, double y)
{
    return (double)nextafterf((float)x, (float)y);
}
