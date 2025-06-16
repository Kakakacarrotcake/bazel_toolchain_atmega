/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef llrintl

long long llrintl(long double x)
{
    return llrintf((float)x);
}
