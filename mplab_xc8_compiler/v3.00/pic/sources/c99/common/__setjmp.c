#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>

#if defined(_PIC18) || defined(_PIC14E) || defined(_PIC14EX)

#pragma warning push
#pragma warning disable 368

enum {
	JMP_BSR		= 0,
#if defined(__STACK) && __STACK !=  __STACK_COMPILED
	JMP_FSR1L,
	JMP_FSR1H,
#endif
	JMP_STKPTR,
	JMP_TOSL,
	JMP_TOSH,
#if	defined(_PIC18) 
	JMP_TOSU,
#endif
};

/* setjmp/longjmp for PIC18 series; all that is required is to
 * reload the stack pointer then return
 */

#ifdef _PIC18
#pragma regsused setjmp wreg status fsr0l bsr pclath pclatu
#else
#pragma regsused setjmp wreg status fsr0l bsr pclath
#endif
#ifdef __STACK
__nonreentrant 
#endif
int 
setjmp(jmp_buf jp) {
	volatile uint8_t bsr;
	/* save context */
#ifdef _PIC18
	asm("movff _BSR,setjmp@bsr");
#else
	asm("movf _BSR,w");
	asm("movlb ((setjmp@bsr/080h)&0xFF)");
	asm("movwf (setjmp@bsr&0x7F)");
#endif
	jp[JMP_BSR] = bsr;
#if defined(__STACK) && __STACK !=  __STACK_COMPILED
	jp[JMP_FSR1L] = FSR1L;
	jp[JMP_FSR1H] = FSR1H;
#endif
	jp[JMP_STKPTR] = STKPTR;
	jp[JMP_TOSL] = TOSL;
	jp[JMP_TOSH] = TOSH;
#ifdef _PIC18
	jp[JMP_TOSU] = TOSU;
#endif
	return 0;
}

#ifdef _PIC18
#pragma regsused longjmp wreg status fsr0l bsr pclath pclatu
#else
#pragma regsused longjmp wreg status fsr0l bsr pclath
#endif
#ifdef __STACK
__nonreentrant 
#endif
void 
longjmp(jmp_buf jp, int retval) {
	/* If retval is non-zero, setjmp will return it, 
	 * else setjmp will return 1.
	 */
	if(retval == 0) {
#ifdef _PIC18
		asm("movlb ((longjmp@retval>>8)&0xFF)");
		asm("movlw 1");
		asm("movwf (longjmp@retval&0xFF)");
		asm("clrf (longjmp@retval+1)&0xFF");
#else
		asm("movlb ((longjmp@retval/080h)&0xFF)");
		asm("movlw 1");
		asm("movwf (longjmp@retval&0x7F)");
		asm("clrf ((longjmp@retval+1)&0x7F)");
#endif
	}
	/* simulate returning a value from setjmp() */

	/* restore context */
	STKPTR	= jp[JMP_STKPTR];
#ifdef _PIC18
	TOSU	= jp[JMP_TOSU];
#endif
	TOSH	= jp[JMP_TOSH];
	TOSL	= jp[JMP_TOSL];
#if defined(__STACK) && __STACK !=  __STACK_COMPILED
	FSR1H	= jp[JMP_FSR1H];
	FSR1L	= jp[JMP_FSR1L];
#endif
	FSR0L	= jp[JMP_BSR];
#ifdef _PIC18
	asm("movff longjmp@retval,?_setjmp");
	asm("movff longjmp@retval+1,?_setjmp+1");
#else
	asm("movlb ((longjmp@retval/080h)&0xFF)");
	asm("movf (longjmp@retval&0x7F),w");
	asm("movlb ((?_setjmp/080h)&0xFF)");
	asm("movwf (?_setjmp&0x7F)");
	asm("movlb ((longjmp@retval/080h)&0xFF)");
	asm("movf (longjmp@retval+1)&0x7F,w");
	asm("movlb ((?_setjmp/080h)&0xFF)");
	asm("movwf (?_setjmp+1)&0x7F");
#endif
	BSR = FSR0L;
}

#pragma warning pop

#endif

