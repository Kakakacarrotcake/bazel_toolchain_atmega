#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include "libc.h"
#include <string.h>

extern void *__simple_malloc(size_t n);

void *__simple_realloc(void *ptr, size_t size) {
	void *new_ptr;

	new_ptr = __simple_malloc(size);
	if (new_ptr && ptr) {
		size_t old_size;
		size_t copy_bytes = size;

		old_size = ((size_t *)ptr)[-1];
		if (old_size < size) copy_bytes = old_size;
		(void) memcpy(new_ptr, ptr, copy_bytes);
		free(ptr);
	}
	return new_ptr;
}

weak_alias(__simple_realloc, realloc);
weak_alias(__simple_realloc, __realloc0);
