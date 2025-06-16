/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef asinh

double asinh(double x)
{
    return (double)asinhf((float)x);
}
