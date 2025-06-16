#include <stdlib.h>
#include <stdint.h>

static uint32_t seed;

void srand(unsigned int s)
{
	seed = s-1;
}

int rand(void)
{
	seed = 1284865837UL*seed + 1;
	return (int)(unsigned int)(seed >> 17);
}
