#include "libm.h"

/* k is such that k*ln2 has minimal relative error and x - kln2 > log(FLT_MIN) */
static const int k = 235;
static const float kln2 = 0x1.45c778p+7f;

/* expf(x)/2 for x >= log(FLT_MAX), slightly better than 0.5f*expf(x/2)*expf(x/2) */
float __expo2f(float x)
{
	union { float f; uint32_t i; } scale;

	/* note that k is odd and scale*scale overflows */
	scale.i = (uint32_t)(0x7f + k/2) << 23;
	/* exp(x - k ln2) * 2**(k-1) */
	return expf(x - kln2) * scale.f * scale.f;
}
