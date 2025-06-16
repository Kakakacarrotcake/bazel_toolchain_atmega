/*	Floating point routines.

	Copyright (c) 2012 Microchip Technology Inc. All rights reserved.	
*/

// this is bigendian code right now. Need to parameterise it.

#include	"ftarith.h"

#pragma warning disable 1516

// Convert signed long long int to float

float
__aotoft(signed long long int c)
{
	unsigned char exp, sign;
	sign = 0;
	exp = 127+15;
	if (c < 0) {
		c = -c;
		sign = 1;
	}
	// normalize 64 bits to 24 first
	while(((unsigned long long)c) & ~0xFFFFFFULL) {
		*(unsigned long long int *)&c >>= 1;
		exp++;
	}
	return __ftpack((__uint24)c, exp, sign); 
}
