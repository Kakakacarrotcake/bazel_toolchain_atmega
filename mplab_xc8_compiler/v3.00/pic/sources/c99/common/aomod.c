// long long signed unsigned modulus

#pragma warning disable 1516

signed long long int
#ifdef __PICC__
__aomod(signed long long int divisor, signed long long int dividend)
#else
__aomod(signed long long int dividend, signed long long int divisor)
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
		while(((unsigned long long)divisor & 0x8000000000000000ULL) == 0) {
			divisor <<= 1;
			counter++;
		}
		do {
			if((unsigned long long)divisor <= (unsigned long long)dividend)
				dividend -= divisor;
			*(unsigned long long int *)&divisor >>= 1;
		} while(--counter != 0);
	}
	if(sign)
		dividend = -dividend;
	return dividend;
}
