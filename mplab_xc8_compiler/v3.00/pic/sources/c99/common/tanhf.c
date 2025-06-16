#include <stdbool.h>
#include "libm.h"

float tanhf(float x)
{
	union {float f; uint32_t i;} ux;
	bool sign;
	float t;

	/* x = |x| */
	ux.f = x;
	sign = ux.i & (UINT32_C(1) << 31);
	ux.i &= 0x7fffffff;

	if (ux.i > 0x3f0c9f54) {
		/* |x| > log(3)/2 ~= 0.5493 or nan */
		if (ux.i > 0x41200000) {
			/* |x| > 10 */
			t = 1 + 0/ux.f;
		} else {
			t = expm1f(2*ux.f);
			t = 1 - 2/(t+2);
		}
	} else if (ux.i > 0x3e82c578) {
		/* |x| > log(5/3)/2 ~= 0.2554 */
		t = expm1f(2*ux.f);
		t = t/(t+2);
	} else if (ux.i >= 0x00800000) {
		/* |x| >= 0x1p-126 */
		t = expm1f(-2*ux.f);
		t = -t/(t+2);
	} else {
		/* |x| is subnormal */
		FORCE_EVAL(ux.f*ux.f);
		// t = ux.f;
		return 0.0f; // XC8-PIC flushes subnorms to zero
	}
	return sign ? -t : t;
}
