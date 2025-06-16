#include <stdbool.h>
#include "libm.h"

#if FLT_EVAL_METHOD==0
#define EPS FLT_EPSILON
#elif FLT_EVAL_METHOD==1
#define EPS DBL_EPSILON
#elif FLT_EVAL_METHOD==2
#define EPS LDBL_EPSILON
#endif
static const float_t toint = 1/EPS;

float roundf(float x)
{
	union {float f; uint32_t i;} u = {x};
	int e = u.i >> 23 & 0xff;
	float_t y, z;
	bool sign;

	if (e >= 0x7f+23)
		return x;
	sign = u.i & (UINT32_C(1) << 31);
	if (sign)
		x = -x;
	if (e < 0x7f-1) {
#if math_errhandling & MATH_ERRNO
		return 0;
#else
		FORCE_EVAL(x + toint);
		return 0*u.f;
#endif
	}
	y = x + toint - toint - x;
	z = 0;
	if (y > 0.5f)
		z = -1;
	else if (y <= -0.5f)
		z = 1;
	y = y + x + z;
	if (sign)
		y = -y;
	return y;
}
