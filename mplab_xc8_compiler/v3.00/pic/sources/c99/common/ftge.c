#define	f1	(*(__uint24 *)&ff1)
#define	f2	(*(__uint24 *)&ff2)
__bit
__ftge(float ff1, float ff2)
{
	if(f1 &  0x800000UL)
		f1 = 0x800000UL - f1;
	if(f2 &  0x800000UL)
		f2 = 0x800000UL - f2;
	f1 ^= 0x800000UL;
	f2 ^= 0x800000UL;
	return f1 >= f2;
}
