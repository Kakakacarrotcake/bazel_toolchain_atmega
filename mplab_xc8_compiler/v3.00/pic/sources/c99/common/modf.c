/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef modf

double modf(double x, double *y)
{
    float fpart, ipart;

    fpart = modff((float)x, &ipart);
    *y = (double)ipart;
    return (double)fpart;
}
