#include <math.h>
#include <stdint.h>

float hypotf(float x, float y)
{
	union {float f; uint32_t i;} ux = {x}, uy = {y}, ut;
	float_t z;

	ux.i &= (-UINT32_C(1))>>1;
	uy.i &= (-UINT32_C(1))>>1;
	if (ux.i < uy.i) {
		ut = ux;
		ux = uy;
		uy = ut;
	}

	x = ux.f;
	y = uy.f;
	if (uy.i == 0xffL<<23)
		return y;
	if (ux.i >= 0xffL<<23 || uy.i == 0 || ux.i - uy.i >= 25L<<23)
		return x + y;

	z = 1;
	if (ux.i >= (0x7fL+60)<<23) {
		z = 0x1p90f;
		x *= 0x1p-90f;
		y *= 0x1p-90f;
	} else if (uy.i < (0x7fL-60)<<23) {
		z = 0x1p-90f;
		x *= 0x1p90f;
		y *= 0x1p90f;
	}
	return z*sqrtf((float)((double)x*x + (double)y*y));
}
