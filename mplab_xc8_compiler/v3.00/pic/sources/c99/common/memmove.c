#include	<string.h>
#include	<stdint.h>

void *
memmove(void * d1, const void * s1, size_t n)
{

	union {
		uintptr_t		i;
		char *			p;
		const char *	cp;
	} d, s;
	char c;
#ifdef _PIC18
#if _ROMSIZE <= 0x20000
#define ADDR_MASK 0x1fffful
#else
#error PIC18 with unsupported ROMSIZE
#endif
#endif

#ifdef __OPTIMIZE_SPEED__
	if (n != 0) {
#endif
		d.i = (uintptr_t)0;
		s.i = (uintptr_t)0;
		s.cp = s1;
		d.p = d1;
#ifdef _PIC18
		if((ADDR_MASK & s.i) < (ADDR_MASK & d.i) && ((ADDR_MASK & s.i)+n) > (ADDR_MASK & d.i)) {
#else
		if(s.i < d.i && (s.i+n) > d.i) {
#endif
				s.cp += n;
				d.p += n;
				do {			/* n != 0 */
					c =  *--s.cp;
					--d.p;
					*d.p = c;
				} while (--n);
		} 
		else 
#ifndef __OPTIMIZE_SPEED__
			 if (n) 
#endif
		{
			do {
				c = *s.cp++;
				*d.p = c;
				d.p++;
			} while (--n);
		}
#ifdef __OPTIMIZE_SPEED__	
	}
#endif
	return d1;
}
