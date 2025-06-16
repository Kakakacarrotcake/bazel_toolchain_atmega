#ifndef _STDLIB_H
#define _STDLIB_H

#include <musl_xc8.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif

#define __NEED_size_t
#define __NEED_wchar_t

#include <bits/alltypes.h>

int atoi (const char *);
long atol (const char *);
#if __SIZEOF_LONG_LONG__ >= 8
long long atoll (const char *);
#endif
double atof (const char *);
#define atof(s)		strtod(s,0)

float strtof (const char *__restrict, char **__restrict);
double strtod (const char *__restrict, char **__restrict);
long double strtold (const char *__restrict, char **__restrict);
#define	strtod(x,y)		strtof(x,y)
#define strtold(x,y)	strtof(x,y)

long strtol (const char *__restrict, char **__restrict, int);
unsigned long strtoul (const char *__restrict, char **__restrict, int);
#if __SIZEOF_LONG_LONG__ >= 8
long long strtoll (const char *__restrict, char **__restrict, int);
unsigned long long strtoull (const char *__restrict, char **__restrict, int);
#endif

unsigned long __strtoxl(const char * s, char ** endptr, int base, char is_signed);
#if __SIZEOF_LONG_LONG__ >= 8
unsigned long long __strtoxll(const char * s, char ** endptr, int base, char is_signed);
#endif
#define strtol(s,endptr,base)	(long)__strtoxl(s,endptr,base,1)
#define strtoul(s,endptr,base)	__strtoxl(s,endptr,base,0)
#if __SIZEOF_LONG_LONG__ >= 8
#define strtoll(s,endptr,base)	(long long)__strtoxll(s,endptr,base,1)
#define strtoull(s,endptr,base)	__strtoxll(s,endptr,base,0)
#endif

int rand (void);
void srand (unsigned);

void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);

_Noreturn void abort (void);
int atexit (void (*) (void));
_Noreturn void exit (int);
_Noreturn void _Exit (int);

void *bsearch (const void *, const void *, size_t, size_t, int (*)(const void *, const void *));

#ifdef _MPC_
#define _QSORT_QUAL	__nonreentrant
#else
#define _QSORT_QUAL
#endif

_QSORT_QUAL void qsort (void *, size_t, size_t, int (*)(const void *, const void *));

int abs (int);
long labs (long);
#if __SIZEOF_LONG_LONG__ >= 8
long long llabs (long long);
#endif

typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
#if __SIZEOF_LONG_LONG__ >= 8
typedef struct { long long quot, rem; } lldiv_t;
#endif

div_t div (int, int);
ldiv_t ldiv (long, long);
#if __SIZEOF_LONG_LONG__ >= 8
lldiv_t lldiv (long long, long long);
#endif

typedef struct { unsigned int quot, rem; } udiv_t;
typedef struct { unsigned long quot, rem; } uldiv_t;
udiv_t udiv (unsigned int, unsigned int);
uldiv_t uldiv (unsigned long, unsigned long);


#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define MB_CUR_MAX 1

#define RAND_MAX (0x7fff)

#ifdef __cplusplus
}
#endif

#endif
