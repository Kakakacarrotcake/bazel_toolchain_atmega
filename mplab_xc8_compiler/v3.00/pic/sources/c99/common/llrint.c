/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef llrint

long long llrint(double x)
{
    return llrintf((float)x);
}
