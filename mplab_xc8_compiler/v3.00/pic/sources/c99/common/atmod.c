// short long signed unsigned modulus

__int24
#ifdef __PICC__
__atmod(__int24 divisor, __int24 dividend)
#else
__atmod(__int24 dividend, __int24 divisor)
#endif
{
	unsigned char	counter, sign;

	sign = 0;
	if(dividend < 0) {
		dividend = -dividend;
		sign = 1;
	}
	if(divisor < 0)
		divisor = -divisor;
	if(divisor != 0) {
		counter = 1;
		while((((__uint24)divisor) & (__uint24)0x800000UL) == 0) {
			divisor <<= 1;
			counter++;
		}
		do {
			if((__uint24)divisor <= (__uint24)dividend)
				dividend -= divisor;
			*(__uint24 *)&divisor >>= 1;
		} while(--counter != 0);
	}
	if(sign)
		dividend = -dividend;
	return dividend;
}
