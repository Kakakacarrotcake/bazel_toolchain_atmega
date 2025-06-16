/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef fmodl

long double fmodl(long double x, long double y)
{
    return (long double)fmodf((float)x, (float)y);
}
