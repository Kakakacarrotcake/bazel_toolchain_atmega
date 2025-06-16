/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef pow

double pow(double x, double y)
{
    return (double)powf((float)x, (float)y);
}
