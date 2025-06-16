/*
 *	Quicksort based on the algorithm given in
 *	"Algorithms + Data Structures = Programs" by N. Wirth.
 */

#include	<stdlib.h>
#include	<string.h>

static void
swap(unsigned len, char * s1, char * s2)
{
	register char	c;

	while(len--) {
		c = *s1;
		*s1++ = *s2;
		*s2++ = c;
	}
}

_QSORT_QUAL void
qsort(void * vbase, size_t nel, size_t width, int (*compar)(const void *, const void *))
{
	char *	base;
	char *	x;
	int	i,j,l,r;
	int	s;
#if defined(__PICC18__)
	struct
	{
		int	l,r;
	}	stack[16];
	char	xbuf[256];
#elif defined(__PICCPRO__)
	struct
	{
		int	l,r;
	}	stack[16];
	char	xbuf[60];
#elif defined(__AVR_ARCH__)
	struct
	{
		int	l,r;
	}	stack[16];
	char	xbuf[60];
#else
#error Unsupported target architecture
#endif

	base = vbase;
	if (width < sizeof xbuf) {
		x = xbuf;
	}
	else {
		return;
	}
	x[width] = 0;
	s = 0;
	stack[0].l = 0;
	stack[0].r = (int)nel-1;
	do {
		/* take top request from stack */
		l = stack[s].l;
		r = stack[s--].r;

		do {
			i = l;
			j = r;
			memcpy(x, base+width*(size_t)((i+j)/2), width);
			do {
				while((*compar)(base+(size_t)i*width, x) < 0)
					i++;
				while((*compar)(x, base+(size_t)j*width) < 0)
					j--;
				if(i <= j) {
					swap(width, base+(size_t)i*width, base+(size_t)j*width);
					i++;
					j--;
				}
			} while(i <= j);
			if(j-l < r-i) {
				if(i < r) {	/* stack right partition */
					stack[++s].l = i;
					stack[s].r = r;
				}
				r = j;		/* continue with left */
			} else {
				if(l < j) {
					stack[++s].l = l;
					stack[s].r = j;
				}
				l = i;
			}
		} while(l < r);
	} while(s >= 0);
	return;
}
