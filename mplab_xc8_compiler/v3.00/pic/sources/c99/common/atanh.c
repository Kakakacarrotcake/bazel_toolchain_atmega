/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef atanh

double atanh(double x)
{
    return (double)atanhf((float)x);
}
