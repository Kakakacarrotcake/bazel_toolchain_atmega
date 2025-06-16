/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef scalbnl

long double scalbnl(long double x, int y)
{
    return (long double)scalbnf((float)x, y);
}
