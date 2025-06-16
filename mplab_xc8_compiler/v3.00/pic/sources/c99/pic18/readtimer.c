/*
 * Stub functions used to express that the macro functions READTIMER0(), READTIMER1() and READTIMER3(), 
 * are not available to the current device. See pic18.h for the unsupported attribute.
 */

unsigned short
__readtimer0(void)
{
	return 0;
}


unsigned short
__readtimer1(void)
{
	return 0;
}

unsigned short
__readtimer3(void)
{
	return 0;
}

