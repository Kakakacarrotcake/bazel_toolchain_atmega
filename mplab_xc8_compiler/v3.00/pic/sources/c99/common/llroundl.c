/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef llroundl

long long llroundl(long double x)
{
    return llroundf((float)x);
}
