// since we use tests on the size of operands, suppress the warnings that
// would be generated
#pragma warning disable 757
#pragma warning disable 1516

/* Convert an integer to a 32-bit floating point value.
 * sign : is val actually a signed value
 * val  : the incoming integer (signed or unsigned) for conversion, cast to a signed long long
 */ 
double
__xotofl(unsigned char sign, signed long long val)
{
	unsigned char exp;
	unsigned long long arg;

	if(sign && val < 0) {
		arg = (unsigned long long) -val;
	}
	else {
		arg = (unsigned long long) val;
	}
	if(val == 0)
		return 0.0;
	exp = (127 + 23);
	while(arg & ~0x1FFFFFFULL) {
		++exp;
		arg >>= 1;
	}
	while(arg & ~0xFFFFFFULL) {
		++exp;
		++arg;
		arg >>= 1;
	}
	while(!(arg & 0x800000ULL) && exp > 1) {
		--exp;
		arg <<= 1;
	}
	if(!(exp & 1))
		arg &= ~0x800000ULL;
	exp >>= 1;
	arg |= (unsigned long long)exp << 24;
	if(sign && val < 0)
		arg |= 0x80000000ULL;
	return *(double *)&arg;
}
