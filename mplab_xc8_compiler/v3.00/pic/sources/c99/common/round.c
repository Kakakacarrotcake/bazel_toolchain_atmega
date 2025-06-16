/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef round

double round(double x)
{
    return (double)roundf((float)x);
}
