unsigned long __strtoxl(const char * s, char ** endptr, int base, char is_signed);

long
strtol(const char * restrict s, char ** restrict endptr, int base)
{
	return (long) __strtoxl(s, endptr, base, 1);
}
