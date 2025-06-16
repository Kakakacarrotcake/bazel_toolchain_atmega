#include	<xc.h>

#if	EEPROM_SIZE > 0

unsigned char
eeprom_read(unsigned char addr)
{
	do
		CLRWDT();

#if _EEPROM_INT == _EEREG_INT
	while(EECON1bits.WR);
#elif _EEPROM_INT == _NVMREG_INT
	while (NVMCON1bits.WR);
#else
#error "Unknonwn EEPROM register interface"
#endif
	return EEPROM_READ(addr);
}
#endif
