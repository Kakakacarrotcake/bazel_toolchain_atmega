/*	Floating point routines.

	Copyright (c) 2012 Microchip Technology Inc. All rights reserved.
*/

// this is bigendian code right now. Need to parameterise it.

#include	"ftarith.h"


// Convert unsigned long long int to float

#pragma warning disable 1516

float
__lotoft(unsigned long long int c)
{
	unsigned char	exp;
	exp = 127+15;

	// normalize 64 bits to 24 first
	while(c & ~0xFFFFFFULL) {
		c >>= 1;
		exp++;
	}
	return __ftpack((__uint24)c, exp, 0);
}
