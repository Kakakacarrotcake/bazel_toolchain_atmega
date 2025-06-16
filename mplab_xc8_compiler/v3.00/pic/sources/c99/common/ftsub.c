/*	Floating point routines.

	Copyright (c) 2012 Microchip Technology Inc. All rights reserved.
*/

// this is bigendian code right now. Need to parameterise it.

#include	"ftarith.h"

#define	f1_as_mant1	(*(__uint24 *)&f1)
#define	f2_as_mant2	(*(__uint24 *)&f2)

// floating addition

float
#ifdef __PICC__
__ftsub(float f2, float f1)
#else
__ftsub(float f1, float f2)
#endif
{
	if (f2 != 0) {
		f2_as_mant2 ^= 0x800000;
	}
	return __ftadd(f1, f2);
}
