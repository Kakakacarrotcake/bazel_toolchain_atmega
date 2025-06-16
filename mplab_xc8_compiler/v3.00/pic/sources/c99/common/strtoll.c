unsigned long long __strtoxll(const char * s, char ** endptr, int base, char is_signed);

long long
strtoll(const char * restrict s, char ** restrict endptr, int base)
{
	return (long long)__strtoxll(s, endptr, base, 1);
}
