/*	Floating point routines.

	Copyright (c) 2012 Microchip Technology Inc. All rights reserved.
*/

// this is bigendian code right now. Need to parameterise it.

#include	"ftarith.h"

#define	mant1	(*(__uint24 *)&f1)

// Negate a float

float
__ftneg(float f1)
{
	if(f1 != 0)
		mant1 ^= 0x800000UL;
	return f1;
}
