/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef scalbn

double scalbn(double x, int y)
{
    return (double)scalbnf((float)x, y);
}
