#include <inline.h>

extern unsigned long long __omul(unsigned long long, unsigned long long);

unsigned long long _INLINE
__asomul(unsigned long long * mp, unsigned long long multiplicand)
{
	return *mp = __omul(*mp, multiplicand);
}

