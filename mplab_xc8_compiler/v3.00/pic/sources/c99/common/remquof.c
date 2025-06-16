#include <math.h>
#include <stdint.h>
#include <stdbool.h>

float remquof(float x, float y, int *quo)
{
	union {float f; uint32_t i;} ux = {x}, uy = {y};
	int ex = ux.i>>23 & 0xff;
	int ey = uy.i>>23 & 0xff;
	bool sx = ux.i & (UINT32_C(1) << 31);
	bool sy = uy.i & (UINT32_C(1) << 31);
	uint32_t q;
	uint32_t i;

	*quo = 0;
	if (uy.i<<1 == 0 || isnan(y) || ex == 0xff) {
#if math_errhandling & MATH_ERRNO
		ux.i = UINT32_C(0x7F8FFFFF);
		return ux.f; // NaN
#else
		return (x*y)/(x*y);
#endif
	}
	if (ux.i<<1 == 0)
		return x;

	/* normalize x and y */
	if (!ex) {
		for (i = ux.i<<9; (i & (UINT32_C(1) << 31)) == 0; ex--, i <<= 1);
		ux.i <<= -ex + 1;
	} else {
		ux.i &= UINT32_C(-1) >> 9;
		ux.i |= UINT32_C(1) << 23;
	}
	if (!ey) {
		for (i = uy.i<<9; (i & (UINT32_C(1) << 31)) == 0; ey--, i <<= 1);
		uy.i <<= -ey + 1;
	} else {
		uy.i &= UINT32_C(-1) >> 9;
		uy.i |= UINT32_C(1) << 23;
	}

	q = 0;
	if (ex < ey) {
		if (ex+1 == ey)
			goto end;
		return x;
	}

	/* x mod y */
	for (; ex > ey; ex--) {
		i = ux.i - uy.i;
		if ((i & (UINT32_C(1)<<31)) == 0) {
			ux.i = i;
			q++;
		}
		ux.i <<= 1;
		q <<= 1;
	}
	i = ux.i - uy.i;
	if ((i & (UINT32_C(1)<<31)) == 0) {
		ux.i = i;
		q++;
	}
	if (ux.i == 0)
		ex = -30;
	else
		for (; (ux.i & UINT32_C(0xFF800000)) == 0; ux.i <<= 1, ex--);
end:
	/* scale result and decide between |x| and |x|-|y| */
	if (ex > 0) {
		ux.i -= UINT32_C(1) << 23;
		ux.i |= (uint32_t)ex << 23;
	} else {
		ux.i >>= -ex + 1;
	}
	if (sy)
		y = -y;
	if (ex == ey || (ex+1 == ey && (2*ux.f > y || (2*ux.f == y && q%2)))) {
		ux.f -= y;
		q++;
	}
	q &= UINT32_C(0x7fffffff);
	*quo = sx^sy ? -(int)q : (int)q;
	return sx ? -ux.f : ux.f;
}
