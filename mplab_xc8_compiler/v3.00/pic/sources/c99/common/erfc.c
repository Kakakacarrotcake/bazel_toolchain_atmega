/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef erfc

double erfc(double x)
{
    return (double)erfcf((float)x);
}
