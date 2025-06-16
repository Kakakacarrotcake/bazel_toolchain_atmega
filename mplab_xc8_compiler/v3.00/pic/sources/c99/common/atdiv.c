// short long signed unsigned division

__int24
#ifdef __PICC__
__atdiv(__int24 divisor, __int24 dividend)
#else
__atdiv(__int24 dividend, __int24 divisor)
#endif
{
	__int24		quotient;
	unsigned char	counter, sign;

	sign = 0;
	if(divisor < 0) {
		divisor = -divisor;
		sign = 1;
	}
	if(dividend < 0) {
		dividend = -dividend;
		sign ^= 1;
	}
	quotient = 0;
	if(divisor != 0) {
		counter = 1;
		while(((__uint24)divisor & (__uint24)0x800000UL) == 0) {
			divisor <<= 1;
			counter++;
		}
		do {
			quotient <<= 1;
			if((__uint24)divisor <= (__uint24)dividend) {
				dividend -= divisor;
				quotient |= 1;
			}
			*(__uint24 *)&divisor >>= 1;
		} while(--counter != 0);
	}
	if(sign)
		quotient = -quotient;
	return quotient;
}
