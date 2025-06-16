/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef acos

double acos(double x)
{
    return (double)acosf((float)x);
}
