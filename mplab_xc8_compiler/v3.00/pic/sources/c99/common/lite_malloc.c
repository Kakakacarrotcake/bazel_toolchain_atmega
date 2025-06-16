#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include "libc.h"

void *__expand_heap(size_t *);

extern void __attribute__((weak)) __simple_free(void *);
extern void  * __attribute__((weak)) __simple_search_fl(size_t n);

#ifndef ALIGN
#define ALIGN(X) ((X+sizeof(void *)-1) & (-sizeof(void*)))
#endif

void *__simple_malloc(size_t n)
{
	static char *cur, *end;
	static volatile int lock[2];
	void *p = NULL;
	size_t size = 0;

	if (!n) {
		n++;
	}

	LOCK(lock);

	if (n < sizeof(intptr_t)) {
		n = sizeof(intptr_t);
	}
	else if ((SIZE_MAX-n) < sizeof(size_t)){
		errno = ENOMEM;
		return NULL;
	}
	size = sizeof(size_t);
	p = __simple_search_fl(n);

	if (p == NULL) {
		if (n > end-cur) {
			size_t m = n + size;
			char *new = __expand_heap(&m);
			if (!new) {
				UNLOCK(lock);
				return 0;
			}
			if (new != end) {
				cur = new;
			}
			end = new + m;
		}
		p = cur;
                if (size) {
                  size_t *sz = p;
                  *sz++ = n;
                  p = sz;
                }
		cur += n + size;
	}

	UNLOCK(lock);
	return p;
}

weak_alias(__simple_malloc, malloc);
weak_alias(__simple_malloc, __malloc0);
