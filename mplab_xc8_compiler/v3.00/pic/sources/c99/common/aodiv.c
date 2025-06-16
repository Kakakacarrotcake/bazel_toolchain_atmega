// long long signed unsigned division

#pragma warning disable 1516

signed long long int
#ifdef __PICC__
__aodiv(signed long long int divisor, signed long long int dividend)
#else
__aodiv(signed long long int dividend, signed long long int divisor)
#endif
{
	signed long long int	quotient;
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
		while(((unsigned long long)divisor & 0x8000000000000000ULL) == 0) {
			divisor <<= 1;
			counter++;
		}
		do {
			quotient <<= 1;
			if((unsigned long long)divisor <= (unsigned long long)dividend) {
				dividend -= divisor;
				quotient |= 1;
			}
			*(unsigned long long int *)&divisor >>= 1;
		} while(--counter != 0);
	}
	if(sign)
		quotient = -quotient;
	return quotient;
}
