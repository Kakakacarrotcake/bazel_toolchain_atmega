/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef erf

double erf(double x)
{
    return (double)erff((float)x);
}
