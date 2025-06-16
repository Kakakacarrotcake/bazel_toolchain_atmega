unsigned long long __strtoxll(const char * s, char ** endptr, int base, char is_signed);

unsigned long long
strtoull(const char * restrict s, char ** restrict endptr, int base)
{
	return __strtoxll(s, endptr, base, 0);
}
