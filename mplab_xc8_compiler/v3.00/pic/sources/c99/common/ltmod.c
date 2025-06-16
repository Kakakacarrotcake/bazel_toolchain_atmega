// short long unsigned unsigned modulus

__uint24
#ifdef __PICC__
__ltmod(__uint24 divisor, __uint24 dividend)
#else
__ltmod(__uint24 dividend, __uint24 divisor)
#endif
{
	unsigned char	counter;

	if(divisor != 0) {
		counter = 1;
		while((divisor & (__uint24)0x800000UL) == 0) {
			divisor <<= 1;
			counter++;
		}
		do {
			if((__uint24)divisor <= (__uint24)dividend)
				dividend -= divisor;
			*(__uint24 *)&divisor >>= 1;
		} while(--counter != 0);
	}
	return dividend;
}
