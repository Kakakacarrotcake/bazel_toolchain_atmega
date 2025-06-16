/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef llround

long long llround(double x)
{
    return llroundf((float)x);
}
