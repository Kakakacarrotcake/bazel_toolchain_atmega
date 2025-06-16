#include <xc.h>
#include <stdarg.h>
#include "itoption.h"

extern volatile __persistent char __DCByte;	// Data capture word for MPLAB to monitor
extern volatile __persistent char __DCDelay;

void __it_trace_dc(char id);
void __it_log_dc(char id, char type, ...);

#pragma interrupt_level 1
#pragma interrupt_level 2
void
__it_trace_dc(char id) {
	__DCByte = id;
	_delay3(__DCDelay);

	return;
}

#pragma interrupt_level 1
#pragma interrupt_level 2
void
__it_log_dc(char id, char type, ...) {
	char *__it_ptr;
	char len;
	long data_var;
	va_list args;

	len = (char)((type & 0x7) + 1);

	va_start(args, type);
	data_var = va_arg(args, long);
	va_end(args);
	__it_ptr = (char *)&data_var;

	__DCByte = type;
	_delay3(__DCDelay);
	__DCByte = id;
	_delay3(__DCDelay);
	do {
		__DCByte = *__it_ptr++;
		_delay3(__DCDelay);
	} while (--len);

	return;
}
