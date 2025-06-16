/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef fminl

long double fminl(long double x, long double y)
{
    return (long double)fminf((float)x, (float)y);
}
