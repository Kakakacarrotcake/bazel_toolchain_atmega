/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef scalblnl

long double scalblnl(long double x, long y)
{
    return (long double)scalblnf((float)x, y);
}
