/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef tanh

double tanh(double x)
{
    return (double)tanhf((float)x);
}
