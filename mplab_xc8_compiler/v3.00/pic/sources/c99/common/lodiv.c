// long long unsigned unsigned division

#pragma warning disable 1516

unsigned long long int
#ifdef __PICC__
__lodiv(unsigned long long int divisor, unsigned long long int dividend)
#else
__lodiv(unsigned long long int dividend, unsigned long long int divisor)
#endif
{
	unsigned long long int	quotient;
	unsigned char	counter;

	quotient = 0;
	if(divisor != 0) {
		counter = 1;
		while((divisor & 0x8000000000000000ULL) == 0) {
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
