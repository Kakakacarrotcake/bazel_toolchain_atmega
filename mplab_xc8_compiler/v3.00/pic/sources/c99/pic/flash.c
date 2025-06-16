/*
 * Stub functions used to express that the macro functions FLASH_READ, FLASH_WRITE and FLASH_ERASE 
 * are no longer supported.
 */

unsigned char
__flash_read(unsigned short addr)
{
	return 0;
}

void
__flash_write(unsigned short addr, unsigned short data)
{
}

void
__flash_erase(unsigned short addr)
{
}
