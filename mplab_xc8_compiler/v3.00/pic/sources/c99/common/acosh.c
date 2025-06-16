/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef acosh

double acosh(double x)
{
    return (double)acoshf((float)x);
}
