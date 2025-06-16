/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef scalbln

double scalbln(double x, long y)
{
    return (double)scalblnf((float)x, y);
}
