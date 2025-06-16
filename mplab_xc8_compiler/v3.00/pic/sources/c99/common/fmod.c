/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef fmod

double fmod(double x, double y)
{
    return (double)fmodf((float)x, (float)y);
}
