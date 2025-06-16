/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef fabs

double fabs(double x)
{
    return (double)fabsf((float)x);
}
