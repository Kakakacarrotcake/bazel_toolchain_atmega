#include <math.h>
#include <stdint.h>
#include <stdbool.h>

float fmodf(float x, float y)
{
	union {float f; uint32_t i;} ux = {x}, uy = {y};
	int ex = (int)(ux.i>>23 & 0xff);
	int ey = (int)(uy.i>>23 & 0xff);
	bool sx = ux.i & 0x80000000;
	uint32_t i;

	if (uy.i<<1 == 0 || isnan(y) || ex == 0xff) {
#if math_errhandling & MATH_ERRNO
		ux.i = UINT32_C(0x7fffffff); // result is NaN
		return ux.f;
#else
		return (x*y)/(x*y);
#endif
	}
	if (ux.i<<1 <= uy.i<<1) {
		if (ux.i<<1 == uy.i<<1) {
#if math_errhandling & MATH_ERRNO
			return 0.0f; // XC8-PIC does not have -0.0f
#else
			return 0*x;
#endif
		}
		return x;
	}

	/* normalize x and y */
	if (!ex) {
		for (i = ux.i<<9; (i & (UINT32_C(1)<<31)) == 0; ex--, i <<= 1);
		ux.i <<= -ex + 1;
	} else {
		ux.i &= UINT32_C(-1) >> 9;
		ux.i |= UINT32_C(1) << 23;
	}
	if (!ey) {
		for (i = uy.i<<9; (i & (UINT32_C(1)<<31)) == 0; ey--, i <<= 1);
		uy.i <<= -ey + 1;
	} else {
		uy.i &= UINT32_C(-1) >> 9;
		uy.i |= UINT32_C(1) << 23;
	}

	/* x mod y */
	for (; ex > ey; ex--) {
		i = ux.i - uy.i;
		if ((i & (UINT32_C(1)<<31)) == 0) {
			if (i == 0) {
#if math_errhandling & MATH_ERRNO
				return 0.0f; // XC8-PIC does not have -0.0f
#else
				return 0*x;
#endif
			}
			ux.i = i;
		}
		ux.i <<= 1;
	}
	i = ux.i - uy.i;
	if ((i & (UINT32_C(1)<<31)) == 0) {
		if (i == 0) {
#if math_errhandling & MATH_ERRNO
			return 0.0f; // XC8-PIC does not have -0.0f
#else
			return 0*x;
#endif
		}
		ux.i = i;
	}
	for (; ux.i>>23 == 0; ux.i <<= 1, ex--);

	/* scale result up */
	if (ex > 0) {
		ux.i -= UINT32_C(1) << 23;
		ux.i |= (uint32_t)ex << 23;
	} else {
		ux.i >>= -ex + 1;
	}
	if (sx && ux.i != 0) { // XC8-PIC has no -0.0f
		ux.i |= UINT32_C(0x80000000);
	}
	return ux.f;
}
