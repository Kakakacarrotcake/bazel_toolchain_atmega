/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef fmin

double fmin(double x, double y)
{
    return (double)fminf((float)x, (float)y);
}
