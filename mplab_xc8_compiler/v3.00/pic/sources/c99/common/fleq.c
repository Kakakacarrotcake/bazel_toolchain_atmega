#define	f1	(*(unsigned long *)&ff1)
#define	f2	(*(unsigned long *)&ff2)
__bit
__fleq(double ff1, double ff2)
{
        // flush denorms and zeroes to +ve 0.0
        if((f1 &  0x7F800000UL)== 0)
		f1 = 0x00000000UL;
	if((f2 &  0x7F800000UL)== 0)
	        f2 = 0x00000000UL;
	return f1 == f2;
}
