// 64 x 64 bit multiplication with 64 bit result
#ifdef _PIC18
// TODO: Disabled for now, still needs work.
#define _Has_hardware_multiply 0
/* note: when the compiler sees the equivalent of

       (unsigned int)(unsigned char)a * (unsigned char)b

         it SHOULD emit a hardware multiply instruction
         which multiplies 2 8-bit values to give a 16-bit result
 */
#else
#define _Has_hardware_multiply 0
#endif

#if defined(_PIC14E) || defined(_PIC14EX) || defined(_PIC18)
// TODO: Disabled for now, still needs work.
#define _Has_large_call_stack 0
#else
#define _Has_large_call_stack 0
#endif

/* FIXME:
   we need *assignable* types
   *without alignment restrictions*
   for each multi-byte integer size */
/* got away with it for 32-bit as we DO have 1, 2, 3, and 4-byte types */
typedef unsigned char T1[1];
typedef unsigned char T2[2];
typedef unsigned char T3[3];
typedef unsigned char T4[4];
typedef unsigned char T5[5];
typedef unsigned char T6[6];
typedef unsigned char T7[7];
typedef unsigned char T8[8];

/* macros to allow us to treat 'x' as an array-of-type-n on arbitrary alignment */
#define B1(x)  ((T1 *)(&x))
#define B2(x)  ((T2 *)(&x))
#define B3(x)  ((T3 *)(&x))
#define B4(x)  ((T4 *)(&x))
#define B5(x)  ((T5 *)(&x))
#define B6(x)  ((T6 *)(&x))
#define B7(x)  ((T7 *)(&x))
#define B8(x)  ((T8 *)(&x))

/* FIXME:
   "+=" operations optimized for each combination of operand sizes
   uchar[n] += uchar[n..1], for each n from 1 to our largest integer type */
/* got away with it for 32-bit as we DO have 1, 2, 3, and 4-byte types */
#define ADDEQ_1_1(a,b)
#define ADDEQ_2_2(a,b)
#define ADDEQ_2_1(a,b)
#define ADDEQ_3_3(a,b)
#define ADDEQ_3_2(a,b)
#define ADDEQ_3_1(a,b)
#define ADDEQ_4_4(a,b)
#define ADDEQ_4_3(a,b)
#define ADDEQ_4_2(a,b)
#define ADDEQ_4_1(a,b)
#define ADDEQ_5_5(a,b)
#define ADDEQ_5_4(a,b)
#define ADDEQ_5_3(a,b)
#define ADDEQ_5_2(a,b)
#define ADDEQ_5_1(a,b)
#define ADDEQ_6_6(a,b)
#define ADDEQ_6_5(a,b)
#define ADDEQ_6_4(a,b)
#define ADDEQ_6_3(a,b)
#define ADDEQ_6_2(a,b)
#define ADDEQ_6_1(a,b)
#define ADDEQ_7_7(a,b)
#define ADDEQ_7_6(a,b)
#define ADDEQ_7_5(a,b)
#define ADDEQ_7_4(a,b)
#define ADDEQ_7_3(a,b)
#define ADDEQ_7_2(a,b)
#define ADDEQ_7_1(a,b)
#define ADDEQ_8_8(a,b)
#define ADDEQ_8_7(a,b)
#define ADDEQ_8_6(a,b)
#define ADDEQ_8_5(a,b)
#define ADDEQ_8_4(a,b)
#define ADDEQ_8_3(a,b)
#define ADDEQ_8_2(a,b)
#define ADDEQ_8_1(a,b)

#pragma warning disable 1516

unsigned long long
__omul(unsigned long long multiplier, unsigned long long multiplicand)
{
	unsigned long long product;

#if (_Has_hardware_multiply || _Has_large_call_stack) && defined(__OPTIMIZE_SPEED__)
	{

#define USE_SHRINK

/*
a 64-bit multiply can be decomposed into the sum of thirty-six 8-bit multiplies
                            a  b  c  d  e  f  g  h
*                           i  j  k  l  m  n  o  p
--------------------------------------------------
                          |                     hp
                          |                  gp  0
                          |               fp  0  0
                          |            ep  0  0  0
                          |         dp  0  0  0  0
                          |      cp  0  0  0  0  0
                          |   bp  0  0  0  0  0  0
                          |ap  0  0  0  0  0  0  0
                          |                  ho  0
                          |               go  0  0
                          |            fo  0  0  0
                          |         eo  0  0  0  0
                          |      do  0  0  0  0  0
                          |   co  0  0  0  0  0  0
                          |bo  0  0  0  0  0  0  0
                        ao| 0  0  0  0  0  0  0  0 (ignore - no effect on low 64)
                          |               hn  0  0
                          |            gn  0  0  0
                          |         fn  0  0  0  0
                          |      en  0  0  0  0  0
                          |   dn  0  0  0  0  0  0
                          |cn  0  0  0  0  0  0  0
                        bn| 0  0  0  0  0  0  0  0 (ignore)
                     an  0| 0  0  0  0  0  0  0  0 (ignore)
                          |            hm  0  0  0
                          |         gm  0  0  0  0
                          |      fm  0  0  0  0  0
                          |   em  0  0  0  0  0  0
                          |dm  0  0  0  0  0  0  0
                        cm| 0  0  0  0  0  0  0  0 (ignore)
                     bm  0| 0  0  0  0  0  0  0  0 (ignore)
                  am  0  0| 0  0  0  0  0  0  0  0 (ignore)
                          |         hl  0  0  0  0
                          |      gl  0  0  0  0  0
                          |   fl  0  0  0  0  0  0
                          |el  0  0  0  0  0  0  0
                        dl| 0  0  0  0  0  0  0  0 (ignore)
                     cl  0| 0  0  0  0  0  0  0  0 (ignore)
                  bl  0  0| 0  0  0  0  0  0  0  0 (ignore)
               al  0  0  0| 0  0  0  0  0  0  0  0 (ignore)
                          |      hk  0  0  0  0  0
                          |   gk  0  0  0  0  0  0
                          |fk  0  0  0  0  0  0  0
                        ek| 0  0  0  0  0  0  0  0 (ignore)
                     dk  0| 0  0  0  0  0  0  0  0 (ignore)
                  ck  0  0| 0  0  0  0  0  0  0  0 (ignore)
               bk  0  0  0| 0  0  0  0  0  0  0  0 (ignore)
            ak  0  0  0  0| 0  0  0  0  0  0  0  0 (ignore)
                          |   hj  0  0  0  0  0  0
                          |gj  0  0  0  0  0  0  0
                        fj| 0  0  0  0  0  0  0  0 (ignore)
                     ej  0| 0  0  0  0  0  0  0  0 (ignore)
                  dj  0  0| 0  0  0  0  0  0  0  0 (ignore)
               cj  0  0  0| 0  0  0  0  0  0  0  0 (ignore)
            bj  0  0  0  0| 0  0  0  0  0  0  0  0 (ignore)
         aj  0  0  0  0  0| 0  0  0  0  0  0  0  0 (ignore)
                          |hi  0  0  0  0  0  0  0
                        gi| 0  0  0  0  0  0  0  0 (ignore)
                     fi  0| 0  0  0  0  0  0  0  0 (ignore)
                  ei  0  0| 0  0  0  0  0  0  0  0 (ignore)
               di  0  0  0| 0  0  0  0  0  0  0  0 (ignore)
            ci  0  0  0  0| 0  0  0  0  0  0  0  0 (ignore)
         bi  0  0  0  0  0| 0  0  0  0  0  0  0  0 (ignore)
      ai  0  0  0  0  0  0| 0  0  0  0  0  0  0  0 (ignore)
==================================================
 */

#if defined(USE_MASKS)
		product = (unsigned int)B1(multiplier)[0] * B1(multiplicand)[0];

		product += ((unsigned long long)
				((unsigned int)B1(multiplier)[0] * B1(multiplicand)[1])
				+
				((unsigned int)B1(multiplier)[1] * B1(multiplicand)[0]))
			<< 8;

		product += ((unsigned long long)
				((unsigned int)B1(multiplier)[0] * B1(multiplicand)[2])
				+
				((unsigned int)B1(multiplier)[1] * B1(multiplicand)[1])
				+
				((unsigned int)B1(multiplier)[2] * B1(multiplicand)[0]))
			<< 16;

		product += ((unsigned long long)
				((unsigned int)B1(multiplier)[0] * B1(multiplicand)[3])
				+
				((unsigned int)B1(multiplier)[1] * B1(multiplicand)[2])
				+
				((unsigned int)B1(multiplier)[2] * B1(multiplicand)[1])
				+
				((unsigned int)B1(multiplier)[3] * B1(multiplicand)[0]))
			<< 24;

		product += ((unsigned long long)
				((unsigned int)B1(multiplier)[0] * B1(multiplicand)[4])
				+
				((unsigned int)B1(multiplier)[1] * B1(multiplicand)[3])
				+
				((unsigned int)B1(multiplier)[2] * B1(multiplicand)[2])
				+
				((unsigned int)B1(multiplier)[3] * B1(multiplicand)[1])
				+
				((unsigned int)B1(multiplier)[4] * B1(multiplicand)[0]))
			<< 32;

		product += ((unsigned long long)
				((unsigned int)B1(multiplier)[0] * B1(multiplicand)[5])
				+
				((unsigned int)B1(multiplier)[1] * B1(multiplicand)[4])
				+
				((unsigned int)B1(multiplier)[2] * B1(multiplicand)[3])
				+
				((unsigned int)B1(multiplier)[3] * B1(multiplicand)[2])
				+
				((unsigned int)B1(multiplier)[4] * B1(multiplicand)[1])
				+
				((unsigned int)B1(multiplier)[5] * B1(multiplicand)[0]))
			<< 40;

		product += ((unsigned long long)
				((unsigned int)B1(multiplier)[0] * B1(multiplicand)[6])
				+
				((unsigned int)B1(multiplier)[1] * B1(multiplicand)[5])
				+
				((unsigned int)B1(multiplier)[2] * B1(multiplicand)[4])
				+
				((unsigned int)B1(multiplier)[3] * B1(multiplicand)[3])
				+
				((unsigned int)B1(multiplier)[4] * B1(multiplicand)[2])
				+
				((unsigned int)B1(multiplier)[5] * B1(multiplicand)[1])
				+
				((unsigned int)B1(multiplier)[6] * B1(multiplicand)[0]))
			<< 48;

		product += ((unsigned long long)
				(B1(multiplier)[0] * B1(multiplicand)[7])
				+
				(B1(multiplier)[1] * B1(multiplicand)[6])
				+
				(B1(multiplier)[2] * B1(multiplicand)[5])
				+
				(B1(multiplier)[3] * B1(multiplicand)[4])
				+
				(B1(multiplier)[4] * B1(multiplicand)[3])
				+
				(B1(multiplier)[5] * B1(multiplicand)[2])
				+
				(B1(multiplier)[6] * B1(multiplicand)[1])
				+
				(B1(multiplier)[7] * B1(multiplicand)[0]))
		<< 56;

		/*
		 * 8 of 8x8->8 multiply
		 * 28 of 8x8->16 multiply
		 * 6 of shift64 (on 8-bit boundaries, potentially optimizable to byte copies)
		 * 35 of 64+64 addeq (each 8 of 8+8 addeq, 28 of carry-prop)
		 * { 280 of 8+8 addeq, 980 of carry-prop }
		 */
#elif  defined(USE_SHRINK)
		/* add direct to upper bytes, rather than shift and add all bytes */

		/* micro-opt: do first 4 8x8->16 multiplies to avoid 3 multi-byte adds */
		B2(product)[0] = (unsigned int)B1(multiplier)[0] * B1(multiplicand)[0];
		B2(product)[1] = (unsigned int)B1(multiplier)[0] * B1(multiplicand)[2];
		B2(product)[2] = (unsigned int)B1(multiplier)[0] * B1(multiplicand)[4];
		B2(product)[3] = (unsigned int)B1(multiplier)[0] * B1(multiplicand)[6];

		ADDEQ_7_2(B7(product)[1], (unsigned int)B1(multiplier)[0] * B1(multiplicand)[1]);
		ADDEQ_5_2(B5(product)[3], (unsigned int)B1(multiplier)[0] * B1(multiplicand)[3]);
		ADDEQ_3_2(B3(product)[5], (unsigned int)B1(multiplier)[0] * B1(multiplicand)[5]);
		ADDEQ_1_1(B1(product)[7], B1(multiplier)[0] * B1(multiplicand)[7]);

		ADDEQ_7_2(B7(product)[1], (unsigned int)B1(multiplier)[1] * B1(multiplicand)[0]);
		ADDEQ_6_2(B6(product)[2], (unsigned int)B1(multiplier)[1] * B1(multiplicand)[1]);
		ADDEQ_5_2(B5(product)[3], (unsigned int)B1(multiplier)[1] * B1(multiplicand)[2]);
		ADDEQ_4_2(B4(product)[4], (unsigned int)B1(multiplier)[1] * B1(multiplicand)[3]);
		ADDEQ_3_2(B3(product)[5], (unsigned int)B1(multiplier)[1] * B1(multiplicand)[4]);
		ADDEQ_2_2(B2(product)[6], (unsigned int)B1(multiplier)[1] * B1(multiplicand)[5]);
		ADDEQ_1_1(B1(product)[7], B1(multiplier)[1] * B1(multiplicand)[6]);

		ADDEQ_6_2(B6(product)[2], (unsigned int)B1(multiplier)[2] * B1(multiplicand)[0]);
		ADDEQ_5_2(B5(product)[3], (unsigned int)B1(multiplier)[2] * B1(multiplicand)[1]);
		ADDEQ_4_2(B4(product)[4], (unsigned int)B1(multiplier)[2] * B1(multiplicand)[2]);
		ADDEQ_3_2(B3(product)[5], (unsigned int)B1(multiplier)[2] * B1(multiplicand)[3]);
		ADDEQ_2_2(B2(product)[6], (unsigned int)B1(multiplier)[2] * B1(multiplicand)[4]);
		ADDEQ_1_1(B1(product)[7], B1(multiplier)[2] * B1(multiplicand)[5]);

		ADDEQ_5_2(B5(product)[3], (unsigned int)B1(multiplier)[3] * B1(multiplicand)[0]);
		ADDEQ_4_2(B4(product)[3], (unsigned int)B1(multiplier)[3] * B1(multiplicand)[1]);
		ADDEQ_3_2(B3(product)[3], (unsigned int)B1(multiplier)[3] * B1(multiplicand)[2]);
		ADDEQ_2_2(B2(product)[3], (unsigned int)B1(multiplier)[3] * B1(multiplicand)[3]);
		ADDEQ_1_1(B1(product)[3], B1(multiplier)[3] * B1(multiplicand)[4]);

		ADDEQ_4_2(B4(product)[4], (unsigned int)B1(multiplier)[4] * B1(multiplicand)[0]);
		ADDEQ_3_2(B3(product)[4], (unsigned int)B1(multiplier)[4] * B1(multiplicand)[1]);
		ADDEQ_2_2(B2(product)[4], (unsigned int)B1(multiplier)[4] * B1(multiplicand)[2]);
		ADDEQ_1_1(B1(product)[4], B1(multiplier)[4] * B1(multiplicand)[3]);

		ADDEQ_3_2(B3(product)[5], (unsigned int)B1(multiplier)[5] * B1(multiplicand)[0]);
		ADDEQ_2_2(B2(product)[5], (unsigned int)B1(multiplier)[5] * B1(multiplicand)[1]);
		ADDEQ_1_1(B1(product)[5], B1(multiplier)[5] * B1(multiplicand)[2]);

		ADDEQ_2_2(B2(product)[6], (unsigned int)B1(multiplier)[6] * B1(multiplicand)[0]);
		ADDEQ_1_1(B1(product)[6], B1(multiplier)[6] * B1(multiplicand)[1]);

		ADDEQ_1_1(B1(product)[7], B1(multiplier)[7] * B1(multiplicand)[0]);

		/*
		 * 8 of 8x8->8 multiply
		 * 28 of 8x8->16 multiply
		 * 8 of 8+8->8 addeq
		 * 6 of 16+16 addeq (each 2 of 8+8 addeq, 1 of carry-prop)
		 * 6 of 24+16 addeq (each 2 of 8+8 addeq, 3 of carry-prop)
		 * 4 of 32+16 addeq (each 2 of 8+8 addeq, 5 of carry-prop)
		 * 4 of 40+16 addeq (each 2 of 8+8 addeq, 7 of carry-prop)
		 * 2 of 48+16 addeq (each 2 of 8+8 addeq, 9 of carry-prop)
		 * 2 of 56+16 addeq (each 2 of 8+8 addeq, 11 of carry-prop)
		 * { 48 of 8+8 addeq, 112 of carry-prop }
		 */
#else
#error No method chosen
#endif
	}
#else

	product = 0;
	do {
		if(multiplier & 1)
			product += multiplicand;
		multiplicand <<= 1;
		multiplier >>= 1;
	} while(multiplier != 0);

	/*
	 * best case:
	 * 1 of clear64
	 * 2 of shift64
	 *
	 * worst case:
	 * 1 of clear64
	 * 64 of 64+64 addop (each 8 of 8+8 addeq, 28 of carry-prop)
	 * { 512 of 8+8 addeq, 1792 of carry-prop }
	 * 128 of shift64
	 */
#endif
	return product;
}
