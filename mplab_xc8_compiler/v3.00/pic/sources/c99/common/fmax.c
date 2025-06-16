/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef fmax

double fmax(double x, double y)
{
    return (double)fmaxf((float)x, (float)y);
}
