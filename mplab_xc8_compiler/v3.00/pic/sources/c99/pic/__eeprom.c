#include	<xc.h>	

#if 	EEPROM_SIZE > 0

void
__eecpymem(volatile unsigned char *to, __eeprom unsigned char * from, unsigned char size)
{
	volatile unsigned char *cp = to;
#if _EEPROM_INT == _EEREG_INT
	while (EECON1bits.WR) continue;
	EEADR = (unsigned char)from;
	while(size--) {
		while (EECON1bits.WR) continue;
		#ifdef	__FLASHTYPE
		EECON1 &= 0x7F;
		#endif
		EECON1bits.RD = 1;
		*cp++ = EEDATA;
		++EEADR;
	}
#elif _EEPROM_INT == _NVMREG_INT
	while (NVMCON1bits.WR) {
		continue;
	}
	NVMCON1bits.NVMREGS = 1;
	NVMADRL = (unsigned char) from;
	NVMADRH = 0x70;
	while (size--) {
		NVMCON1bits.RD = 1;
		*cp++ = NVMDATL;
		NVMADRL++;
	}
#else
#error "Unknonwn EEPROM register interface"
#endif
}

void
__memcpyee(__eeprom unsigned char * to, const unsigned char *from, unsigned char size)
{
	const unsigned char *ptr =from;
#if _EEPROM_INT == _EEREG_INT
	while (EECON1bits.WR) continue;
	EEADR = (unsigned char)to - 1U;
	#ifdef	__FLASHTYPE
	EECON1 &= 0x7F;
	#endif
	while(size--) {
		while (EECON1bits.WR) { 
			continue;
		}
		EEDATA = *ptr++;
		++EEADR;
		STATUSbits.CARRY = 0; 
		if (INTCONbits.GIE) {
			STATUSbits.CARRY = 1; 
		}
		INTCONbits.GIE = 0;
		EECON1bits.WREN = 1;
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1bits.WR = 1;
		EECON1bits.WREN = 0;
		if (STATUSbits.CARRY) {
			INTCONbits.GIE = 1;	//an opportunity for interrupts to happen
		}
	}
#elif _EEPROM_INT == _NVMREG_INT
	while (NVMCON1bits.WR) {
		continue;
	}
	NVMCON1bits.NVMREGS = 1;
	NVMADRL = (unsigned char) to - 1U;
	NVMADRH = 0x70;
	NVMDATH = 0;
	while (size--) {
		while (NVMCON1bits.WR) {
			continue;
		}
		NVMDATL = *ptr++;
		NVMADRL++;
		STATUSbits.CARRY = 0;
		if (INTCONbits.GIE) {
			STATUSbits.CARRY = 1;
		}
		NVMCON1bits.WREN = 1;
		NVMCON2 = 0x55;
		NVMCON2 = 0xAA;
		NVMCON1bits.WR = 1;
		while (NVMCON1bits.WR) {
			continue;
		}
		NVMCON1bits.WREN = 0;
		if (STATUSbits.CARRY) {
			INTCONbits.GIE = 1;
		}
	}
#else
#error "Unknonwn EEPROM register interface"
#endif
}

unsigned char
__eetoc(__eeprom void *addr)
{
	unsigned char data;
	__eecpymem((unsigned char *) &data,addr,1);
	return data;
}

unsigned int
__eetoi(__eeprom void *addr)
{
	unsigned int data;
	__eecpymem((unsigned char *) &data,addr,2);
	return data;
}

#pragma warning push
#pragma warning disable 2040
__uint24
__eetom(__eeprom void *addr)
{
	__uint24 data;
	__eecpymem((unsigned char *) &data,addr,3);
	return data;
}
#pragma warning pop

unsigned long
__eetol(__eeprom void *addr)
{
	unsigned long data;
	__eecpymem((unsigned char *) &data,addr,4);
	return data;
}

#pragma warning push
#pragma warning disable 1516
unsigned long long
__eetoo(__eeprom void *addr)
{
	unsigned long long data;
	__eecpymem((unsigned char *) &data,addr,8);
	return data;
}
#pragma warning pop

unsigned char
__ctoee(__eeprom void *addr, unsigned char data)
{
	__memcpyee(addr,(unsigned char *) &data,1);
	return data;
}

unsigned int
__itoee(__eeprom void *addr, unsigned int data)
{
	__memcpyee(addr,(unsigned char *) &data,2);
	return data;
}

#pragma warning push
#pragma warning disable 2040
__uint24
__mtoee(__eeprom void *addr, __uint24 data)
{
	__memcpyee(addr,(unsigned char *) &data,3);
	return data;
}
#pragma warning pop

unsigned long
__ltoee(__eeprom void *addr, unsigned long data)
{
	__memcpyee(addr,(unsigned char *) &data,4);
	return data;
}

#pragma warning push
#pragma warning disable 1516
unsigned long long
__otoee(__eeprom void *addr, unsigned long long data)
{
	__memcpyee(addr,(unsigned char *) &data,8);
	return data;
}
#pragma warning pop

float
__eetoft(__eeprom void *addr)
{
	float data;
	__eecpymem((unsigned char *) &data,addr,3);
	return data;
}

double
__eetofl(__eeprom void *addr)
{
	double data;
	__eecpymem((unsigned char *) &data,addr,4);
	return data;
}

float
__fttoee(__eeprom void *addr, float data)
{
	__memcpyee(addr,(unsigned char *) &data,3);
	return data;
}

double
__fltoee(__eeprom void *addr, double data)
{
	__memcpyee(addr,(unsigned char *) &data,4);
	return data;
}
#endif
