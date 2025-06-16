#ifndef _LANGUAGE_SUPPORT_H_
#define _LANGUAGE_SUPPORT_H_

#define __at(x) __attribute__((address(x)))
#define __macroexpand(...) (#__VA_ARGS__)
#define __interrupt(...) __attribute__((picinterrupt(__macroexpand(__VA_ARGS__))))
#define __section(name) __attribute__((section(name)))
#define __nonreentrant __attribute__((nonreentrant))
#define __reentrant __attribute__((reentrant))
#define __compiled __attribute__((nonreentrant))
#define __software __attribute__((reentrant))
#define inline __attribute__((inline))
#define __minocg __attribute__((minocg))
#define __fastcall __attribute__((fastcall))
#define asm(arg)	__asm(arg)

#define __bank(arg)	__bank##arg

#define	__deprecate	__attribute__((deprecated))
#define __cp0(arg)	__attribute__((unsupported("the __cp0() construct is not supported by this architecture")))
#define __abi(arg)	__attribute__((unsupported("the __abi() construct is not supported by this architecture")))
#define __align(arg)	__attribute__((unsupported("the __align() attribute is not used by this compiler")))
#define	__xdata		__attribute__((unsupported("__xdata is not defined on this architecture")))
#define	__ydata		__attribute__((unsupported("__ydata in not defined on this architecture")))

#define __invariant	__attribute__((unsupported("invariant mode optimizations are no longer available; specifier will be ignored")))
#define __stable	__attribute__((unsupported("stable mode optimizations are no longer available; specifier will be ignored")))

// set _HTC_EDITION_ and _XC8_MODE_ macros for backwards compatibility
#if     (!defined(_HTC_EDITION_) || !defined(_XC8_MODE_)) && defined(__OPTIM_LEVEL)
#undef _HTC_EDITION_
#undef _XC8_MODE_
#if     __OPTIM_LEVEL < 2
#define _HTC_EDITION_   0
#define _XC8_MODE_      0
#elif   __OPTIM_LEVEL > 2
#define _HTC_EDITION_   2
#define _XC8_MODE_      2
#else
#define _HTC_EDITION_   1
#define _XC8_MODE_      1
#endif
#endif

#if defined(__STACK) && __STACK != __STACK_COMPILED
extern char * __stack_lo;
extern char * __stack_hi;
#if defined(_PIC14E) || defined(_PIC14EX)
extern char * __int_stack_lo;
extern char * __int_stack_hi;
#endif
#if defined(_PIC18) || defined(_PIC18V) || defined(_PIC18XV)
extern char * __inthi_stack_lo;
extern char * __inthi_stack_hi;
extern char * __intlo_stack_lo;
extern char * __intlo_stack_hi;
#endif
#endif

#endif	//_LANGUAGE_SUPPORT_H_

