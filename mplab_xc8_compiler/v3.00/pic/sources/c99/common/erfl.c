/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef erfl

long double erfl(long double x)
{
    return (long double)erff((float)x);
}
