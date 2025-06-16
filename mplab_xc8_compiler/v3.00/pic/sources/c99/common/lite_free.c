#include <stdint.h>
#include <stddef.h>
#include "libc.h"

void *__simple_search_fl(size_t n);
void __simple_free(void *);

struct chunk {
	size_t size;
	union {
		struct chunk *next;
		char space[1];
	};
};

/* we might want to hide this, but its useful to export it */
struct chunk *__freelist = NULL;

void *__simple_search_fl(size_t n) {
	struct chunk *best = NULL;
	struct chunk *best_last = NULL;
	struct chunk *last = NULL;
	struct chunk *c;

	if (__freelist == NULL) return NULL; 

	for (c = __freelist; c; last = c, c = c->next ) {
		if (c->size == n) {
			// exact match... pick it and stop
			if (last) {
				last->next = c->next;
			} else {
				__freelist = c->next;
			}
			return &c->space;
		}
		if (c->size > n) {
			if ((best) && (best->size <= c->size)) continue;
			best = c;
            best_last = last;
		}
	}

	if (best) {
		struct chunk *next;
		next = best->next;

		// did not find exact match, but found one that is 'best'
		if (best->size - n> (2*sizeof(size_t))) {
			struct chunk *split;
	
			// split the block
			size_t remaining_size = best->size - n - sizeof(size_t);
			best->size = n;
			split = (struct chunk *)(&best->space + best->size);
			split->size = remaining_size;
			split->next = next;
            next = split;
		} 
        if (__freelist == best) __freelist = next;
        else best_last->next = next;
		return &best->space;
	}
	return NULL;
}

void __simple_free(void *ptr) {
	struct chunk *c;
	char *cptr = ptr;

	if (ptr == NULL) {
		return;
	}

	c = (struct chunk *)(cptr - sizeof(size_t));
	if (__freelist) {
		if (cptr + c->size == (char*)__freelist) {
			// ptr immediately preceeds __freelist
			c->next = __freelist->next;
			c->size += __freelist->size + sizeof(size_t);
			__freelist = c;
			return;
		} else if ((char *)__freelist + __freelist->size + sizeof(size_t) == (char*)c) {
			// __freelist immediately preceeds ptr
			__freelist->size += c->size + sizeof(size_t);
			return;
		}
		c->next = __freelist;
	}
    else {
        c->next = NULL;
    }
	__freelist = c;
}
	
weak_alias(__simple_free, free);
weak_alias(__simple_free, __free0);
