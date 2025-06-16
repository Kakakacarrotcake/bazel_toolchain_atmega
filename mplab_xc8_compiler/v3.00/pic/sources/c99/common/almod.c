// long signed unsigned modulus

signed long int
#ifdef __PICC__
__almod(signed long int divisor, signed long int dividend)
#else
__almod(signed long int dividend, signed long int divisor)
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
		while(((unsigned long)divisor & 0x80000000UL) == 0) {
			divisor <<= 1;
			counter++;
		}
		do {
			if((unsigned long)divisor <= (unsigned long)dividend)
				dividend -= divisor;
			*(unsigned long int *)&divisor >>= 1;
		} while(--counter != 0);
	}
	if(sign)
		dividend = -dividend;
	return dividend;
}
