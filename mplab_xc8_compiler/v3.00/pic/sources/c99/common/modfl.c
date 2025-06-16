/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef modfl

long double modfl(long double x, long double *y)
{
    float fpart, ipart;

    fpart = modff((float)x, &ipart);
    *y = (long double)ipart;
    return (long double)fpart;
}
