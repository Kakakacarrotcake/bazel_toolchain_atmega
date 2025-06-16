#ifndef	_CTYPE_H
#define	_CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

int   isalnum(int);
int   isalpha(int);
int   isblank(int);
int   iscntrl(int);
int   isdigit(int);
int   isgraph(int);
int   islower(int);
int   isprint(int);
int   ispunct(int);
int   isspace(int);
int   isupper(int);
int   isxdigit(int);
int   tolower(int);
int   toupper(int);

#ifndef __cplusplus
#define isalpha(a) ((((unsigned char)(((a)|32)-'a'))) < 26)
#define isdigit(a) (((unsigned char)((a)-'0')) < 10)
#define isalnum(a) (isalpha(a) || isdigit(a))
#define islower(a) (((unsigned char)((a)-'a')) < 26)
#define isupper(a) (((unsigned char)((a)-'A')) < 26)
#define isprint(a) (((unsigned char)((a)-0x20)) < 0x5f)
#define isgraph(a) (((unsigned char)((a)-0x21)) < 0x5e)
#define isspace(a) (((a == ' ') || ((unsigned char)((a)-'\t') < 5)))
#define isblank(a) ((a) == ' ' || (a) == '\t')
#define iscntrl(a) (((unsigned)(a) < 0x20) || ((a) == 0x7f))
#define ispunct(a) (isgraph(a) && !isalnum(a))
#define isxdigit(a) (isdigit(a) || ((unsigned char)((a|32) - 'a') < 6))
#define tolower(a) (isupper(a) ? (a)|0x20 : (a))
#define toupper(a) (islower(a) ? (a)&0x5f : (a))
#endif


#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)

#define __NEED_locale_t
#include <bits/alltypes.h>

int   isalnum_l(int, locale_t);
int   isalpha_l(int, locale_t);
int   isblank_l(int, locale_t);
int   iscntrl_l(int, locale_t);
int   isdigit_l(int, locale_t);
int   isgraph_l(int, locale_t);
int   islower_l(int, locale_t);
int   isprint_l(int, locale_t);
int   ispunct_l(int, locale_t);
int   isspace_l(int, locale_t);
int   isupper_l(int, locale_t);
int   isxdigit_l(int, locale_t);
int   tolower_l(int, locale_t);
int   toupper_l(int, locale_t);

#define _tolower(a) ((a)|0x20)
#define _toupper(a) ((a)&0x5f)

#endif

#ifdef __cplusplus
}
#endif

#endif
