#include <stdlib.h>
#include <stdint.h>
#include "libc.h"
#include <xc8debug.h>

void __funcs_on_exit(void);

_Noreturn void exit(int code)
{
	__funcs_on_exit();
	__debug_break();
	_Exit(code);
}
