#ifndef _STDIO_IMPL_H
#define _STDIO_IMPL_H

#include <stdio.h>
#include <syscall.h>
#include "libc.h"

#define STDIO_NO_FILE_IO

#ifdef stdin
#undef stdin
#endif
#define stdin	(NULL)
#ifdef stdout
#undef stdout
#endif
#define stdout	(NULL)
#ifdef stderr
#undef stderr
#endif
#define stderr	(NULL)

#define STDIO_NO_BUFFER

#ifndef STDIO_NO_BUFFER
#define UNGET 8
#else
#define UNGET 3
#ifdef BUFSIZ
#undef BUFSIZ
#endif
#define BUFSIZ 0
#endif

#define STDIO_NO_LOCKING
#ifndef STDIO_NO_LOCKING
#define FFINALLOCK(f) ((f)->lock>=0 ? __lockfile((f)) : 0)
#define FLOCK(f) int __need_unlock = ((f)->lock>=0 ? __lockfile((f)) : 0)
#define FUNLOCK(f) if (__need_unlock) __unlockfile((f)); else
#else
#define FFINALLOCK(f)
#define FLOCK(f)
#define FUNLOCK(f)
#endif

#define F_PERM 1
#define F_NORD 4
#define F_NOWR 8
#define F_EOF 16
#define F_ERR 32
#define F_SVB 64
#define F_APP 128

#ifdef STDIO_NO_FILE_IO
extern char __ungetbuf[UNGET];
extern char __ungetcnt;
#define _NF_GETC()	( __ungetcnt ? __ungetbuf[--__ungetcnt] : getch() )
struct _IO_FILE {
	union {
		char * buffer;
		const char * source;
	};
    int count;
	char ungetbuf[UNGET];
	int ungetcnt;
    int limit;
};
#else
struct _IO_FILE {
	unsigned flags;
	unsigned char *rpos, *rend;
	int (*close)(FILE *);
	unsigned char *wend, *wpos;
	unsigned char *mustbezero_1;
	unsigned char *wbase;
	size_t (*read)(FILE *, unsigned char *, size_t);
	size_t (*write)(FILE *, const unsigned char *, size_t);
	off_t (*seek)(FILE *, off_t, int);
	unsigned char *buf;
	size_t buf_size;
	FILE *prev, *next;
	int fd;
	int pipe_pid;
	long lockcount;
	short dummy3;
	signed char mode;
	signed char lbf;
	volatile int lock;
	volatile int waiters;
	void *cookie;
	off_t off;
	char *getln_buf;
	void *mustbezero_2;
	unsigned char *shend;
	off_t shlim, shcnt;
	FILE *prev_locked, *next_locked;
	struct __locale_struct *locale;
};
#endif

size_t __stdio_read(FILE *, unsigned char *, size_t);
size_t __stdio_write(FILE *, const unsigned char *, size_t);
size_t __stdout_write(FILE *, const unsigned char *, size_t);
off_t __stdio_seek(FILE *, off_t, int);
int __stdio_close(FILE *);

size_t __string_read(FILE *, unsigned char *, size_t);

int __toread(FILE *);
int __towrite(FILE *);

#if defined(__PIC__) && (100*__GNUC__+__GNUC_MINOR__ >= 303)
__attribute__((visibility("protected")))
#endif
int __overflow(FILE *, int), __uflow(FILE *);

int __fseeko(FILE *, off_t, int);
int __fseeko_unlocked(FILE *, off_t, int);
off_t __ftello(FILE *);
off_t __ftello_unlocked(FILE *);
size_t __fwritex(const unsigned char *, size_t, FILE *);
int __putc_unlocked(int, FILE *);

FILE *__fdopen(int, const char *);
long __fmodeflags(const char *);

FILE *__ofl_add(FILE *f);
FILE **__ofl_lock(void);
void __ofl_unlock(void);

#define feof(f) ((f)->flags & F_EOF)
#define ferror(f) ((f)->flags & F_ERR)

#ifndef STDIO_NO_LOCKING
#define getc_unlocked(f) \
	( ((f)->rpos < (f)->rend) ? *(f)->rpos++ : __uflow((f)) )

#define putc_unlocked(c, f) \
	( ((unsigned char)(c)!=(f)->lbf && (f)->wpos<(f)->wend) \
	? *(f)->wpos++ = (c) : __overflow((f),(c)) )
#else
#define getc_unlocked(f) fgetc(f)
#define putc_unlocked(c, f) fputc(c, f)
#endif

/* Caller-allocated FILE * operations */
FILE *__fopen_rb_ca(const char *, FILE *, unsigned char *, size_t);
int __fclose_ca(FILE *);

#endif
