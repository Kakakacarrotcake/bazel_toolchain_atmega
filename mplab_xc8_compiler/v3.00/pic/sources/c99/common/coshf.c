#include <errno.h>
#include "libm.h"

float coshf(float x)
{
	union {float f; uint32_t i;} u;
	float t;

	/* |x| */
	u.f = x;
	u.i &= 0x7fffffff;

	/* |x| < log(2) */
	if (u.i < 0x3f317217) {
		if (u.i < 0x3f800000 - (UINT32_C(12)<<23)) {
			FORCE_EVAL(u.f + 0x1p120f);
			return 1;
		}
		t = expm1f(u.f);
		return 1 + t*t/(2*(1+t));
	}

	/* |x| < log(FLT_MAX) */
	if (u.i < 0x42b17217) {
		t = expf(u.f);
		return 0.5f*(t + 1/t);
	}

	/* |x| > log(FLT_MAX) or nan */
	errno = ERANGE;
	return __expo2f(u.f);
}
