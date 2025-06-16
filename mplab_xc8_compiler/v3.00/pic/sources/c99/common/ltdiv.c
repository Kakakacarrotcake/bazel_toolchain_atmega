// short long unsigned unsigned division

__uint24
#ifdef __PICC__
__ltdiv(__uint24 divisor, __uint24 dividend)
#else
__ltdiv(__uint24 dividend, __uint24 divisor)
#endif
{
	__uint24	quotient;
	unsigned char	counter;

	quotient = 0;
	if(divisor != 0) {
		counter = 1;
		while((divisor & (__uint24)0x800000UL) == 0) {
			divisor <<= 1;
			counter++;
		}
		do {
			quotient <<= 1;
			if(divisor <= dividend) {
				dividend -= divisor;
				quotient |= 1;
			}
			divisor >>= 1;
		} while(--counter != 0);
	}
	return quotient;
}
