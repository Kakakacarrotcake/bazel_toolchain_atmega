#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include "libc.h"
#include <string.h>

extern void *__simple_malloc(size_t n);

void *__simple_calloc(size_t nmemb, size_t size) {
	void *new_ptr;

	new_ptr = __simple_malloc(nmemb*size);
	if (new_ptr) {
		(void) memset(new_ptr, 0, nmemb*size);
	}
	return new_ptr;
}

weak_alias(__simple_calloc, calloc);
weak_alias(__simple_calloc, __calloc0);
