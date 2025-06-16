#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#pragma warning push
#pragma warning disable 2028
extern char __heap_lo[], __heap_hi[];
#pragma warning pop

#define MIN_SIZE (sizeof(size_t) + sizeof(intptr_t))

static char * brkp = NULL;

void *__expand_heap(size_t *pn)
{
	size_t n = *pn;
    char * new_brkp;

	if (brkp == NULL) {
		brkp = __heap_hi;
	}
    if (n < MIN_SIZE) {
        n = MIN_SIZE;
    }
    new_brkp = brkp - n;
	if (new_brkp < __heap_lo || new_brkp >= brkp) {
		errno = ENOMEM;
		return NULL;
	}
	brkp = new_brkp;
	return brkp;
}

