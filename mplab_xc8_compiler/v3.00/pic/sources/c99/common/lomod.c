// long long unsigned unsigned modulus

#pragma warning disable 1516

unsigned long long int
#ifdef __PICC__
__lomod(unsigned long long int divisor, unsigned long long int dividend)
#else
__lomod(unsigned long long int dividend, unsigned long long int divisor)
#endif
{
	unsigned char	counter;

	if(divisor != 0) {
		counter = 1;
		while((divisor & 0x8000000000000000ULL) == 0) {
			divisor <<= 1;
			counter++;
		}
		do {
			if((unsigned long long)divisor <= (unsigned long long)dividend)
				dividend -= divisor;
			*(unsigned long long int *)&divisor >>= 1;
		} while(--counter != 0);
	}
	return dividend;
}
