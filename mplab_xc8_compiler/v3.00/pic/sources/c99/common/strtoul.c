unsigned long __strtoxl(const char * s, char ** endptr, int base, char is_signed);

unsigned long
strtoul(const char * restrict s, char ** restrict endptr, int base)
{
	return __strtoxl(s, endptr, base, 0);
}

